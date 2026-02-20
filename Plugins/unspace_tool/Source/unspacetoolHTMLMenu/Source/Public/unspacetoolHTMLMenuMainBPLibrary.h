#pragma once

#include "unspacetoolHTMLMenuMain.h"
#include "unspacetoolHTMLMenu/Source/JSON/FRowData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "unspacetoolHTMLMenuMainBPLibrary.generated.h"


class FunspacetoolHTMLMenuWebsocketClientThread;
class FunspacetoolHTMLMenuWebsocketServerThread;


UCLASS()
class UunspacetoolHTMLMenuMainBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	static UunspacetoolHTMLMenuMainBPLibrary* unspacetoolHTMLMenuMainBPLibrary;
	static UunspacetoolHTMLMenuMainBPLibrary* getunspacetoolHTMLMenuMainBPLibrary();
	   
	void initWebsocketServer(UunspacetoolHTMLMenu* menu);
	void callJavascript(FString javascript);
	void handleWebsocketData(FString& data);
	UunspacetoolHTMLMenu* getMenu();
	void setInConnectionProcess(bool connectionp);

	static TSharedPtr<FJsonObject> ParseJsonString(const FString& JsonString);

	UFUNCTION(BlueprintCallable, Category = "unspacetoolHTMLUtils")
	static FRowData GetRowDataFromString(FString JsonString);
	
	

private:
	class FunspacetoolHTMLMenuWebsocketServerThread* websocketServerThread = nullptr;
	UunspacetoolHTMLMenu* menu = nullptr;
	bool isInConnectionProcess = false;
};


class FunspacetoolHTMLMenuWebsocketClientThread : public FRunnable {

public:
	FunspacetoolHTMLMenuWebsocketClientThread(FSocket* clientSocketP, UunspacetoolHTMLMenuMainBPLibrary * mainlibP) :
		clientSocket(clientSocketP),
		mainlib(mainlibP) {
		FString threadName = "FunspacetoolHTMLMenuWebsocketClientThread_" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override {

		if (mainlib != nullptr)
			mainlib->setInConnectionProcess(false);

		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("unspacetoolHTMLMenu: 20"));*/

			//UE_LOG(LogTemp, Display, TEXT("TCP Connected: %s:%i"), *session.ip, session.port);
		uint32 dataSize;
		TArray<uint8> dataArrayFromSocket;
		TArray<uint8> dataArrayFromSocketToSendToUE;
		while (run && clientSocket != nullptr) {
			//ESocketConnectionState::SCS_Connected does not work https://issues.unrealengine.com/issue/UE-27542
			//Compare ticks is a workaround to get a disconnect. clientSocket->Wait() stop working after disconnect. (Another bug?)
			//If it doesn't wait any longer, ticks1 and ticks2 should be the same == disconnect.
			int64 ticks1 = FDateTime::Now().GetTicks();
			clientSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1));
			int64 ticks2 = FDateTime::Now().GetTicks();
			bool hasData = clientSocket->HasPendingData(dataSize);
			if (!hasData && ticks1 == ticks2)
				break;
			if (hasData) {

				dataArrayFromSocket.Empty();
				dataArrayFromSocket.AddUninitialized(dataSize);
				int32 BytesRead = 0;
				if (clientSocket->Recv(dataArrayFromSocket.GetData(), dataArrayFromSocket.Num(), BytesRead)) {

					if (needHandshake) {

						dataArrayFromSocket.Add(0x00);
						FString recvMessage = FString(UTF8_TO_TCHAR(dataArrayFromSocket.GetData()));

						/*if (GEngine)
							GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, FString::Printf(TEXT("unspacetoolHTMLMenu: 25: %s"), *recvMessage));*/

							//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket get Handshake Data: %s"), *recvMessage);

						TArray<FString> dataLines;
						recvMessage.ParseIntoArray(dataLines, TEXT("\r\n"), true);
						for (int i = 0; i < dataLines.Num(); i++) {
							if (dataLines[i].StartsWith("Sec-WebSocket-Key")) {
								FString clientKey = dataLines[i].Replace(TEXT("Sec-WebSocket-Key: "), TEXT(""));
								//clientKey = recvMessage.Replace(TEXT("\r"), TEXT(""));
								//clientKey = recvMessage.Replace(TEXT("\n"), TEXT(""));
								FString handshake = generateHandshakeData(clientKey);
								//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket send Handshake Data: %s"), *handshake);

								FTCHARToUTF8 Convert(*handshake);
								int32 sent = 0;
								clientSocket->Send((uint8*)((ANSICHAR*)Convert.Get()), Convert.Length(), sent);
								needHandshake = false;

								/*if (GEngine)
									GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, FString::Printf(TEXT("unspacetoolHTMLMenu: 22: %s"), *handshake));*/
								break;
							}
						}
						continue;
					}
					//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket Data 1"));
					//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket Data: encoded: %i "), dataArrayFromSocket.Num());

					TArray<uint8> data;
					TArray<uint8> key;
					dataArrayFromSocketToSendToUE.Append(dataArrayFromSocket.GetData(), dataArrayFromSocket.Num());
					while (dataArrayFromSocketToSendToUE.Num() >= 6) {
						//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket Data 2"));

						if (dataArrayFromSocketToSendToUE[0] != 129) {
							UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu Websocket Data cannot be processed(2)."));
							dataArrayFromSocketToSendToUE.Empty();
							break;
						}

						data.Empty();
						key.Empty();
						uint64 firstDataByteIndex = 0;

						uint64 websocketFrameDataSize = (uint64)dataArrayFromSocketToSendToUE[1] << 0;
						//FMemory::Memcpy(&websocketFrameDataSize, dataArrayFromSocketToSendToUE.GetData() + 1, 1);

						websocketFrameDataSize -= 128;

						if (websocketFrameDataSize > 0 && websocketFrameDataSize <= 125) {
							key.Append(dataArrayFromSocketToSendToUE.GetData() + 2, 4);
							firstDataByteIndex = 6;
						}
						else {
							if (websocketFrameDataSize == 126) {
								websocketFrameDataSize = (uint64)dataArrayFromSocketToSendToUE[2] << 8 | (uint64)dataArrayFromSocketToSendToUE[3] << 0;
								key.Append(dataArrayFromSocketToSendToUE.GetData() + 4, 4);
								firstDataByteIndex = 8;
							}
							else {
								//not tested
								if (websocketFrameDataSize == 127) {
									websocketFrameDataSize = (uint64)dataArrayFromSocketToSendToUE[2] << 56 | (uint64)dataArrayFromSocketToSendToUE[3] << 48 | (uint64)dataArrayFromSocketToSendToUE[4] << 40 | (uint64)dataArrayFromSocketToSendToUE[5] << 32 | (uint64)dataArrayFromSocketToSendToUE[6] << 24 | (uint64)dataArrayFromSocketToSendToUE[7] << 16 | (uint64)dataArrayFromSocketToSendToUE[8] << 8 | (uint64)dataArrayFromSocketToSendToUE[9] << 0;
									key.Append(dataArrayFromSocketToSendToUE.GetData() + 10, 4);
									firstDataByteIndex = 14;
								}
							}
						}

						//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket websocketFrameDataSize: %i"), websocketFrameDataSize);

						if (dataArrayFromSocketToSendToUE.Num() < (websocketFrameDataSize + firstDataByteIndex)){
							break;
						}

						data.Append(dataArrayFromSocketToSendToUE.GetData()+ firstDataByteIndex, websocketFrameDataSize);
						dataArrayFromSocketToSendToUE.RemoveAt(0, websocketFrameDataSize+firstDataByteIndex);


						if (key.Num() == 4 && firstDataByteIndex > 0) {
							//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket Data 6"));
							//decode data to string
							TArray<uint8> decoded;
							int32 j = 0;
							for (int32 i = 0; i < data.Num(); i++) {
								uint8 b = key[j];
								decoded.Add(data[i] ^ b);
								j++;
								if (j > 3)
									j = 0;
							}
							decoded.Add(0x00);
							FString recvMessage = FString(UTF8_TO_TCHAR(decoded.GetData()));
							//UE_LOG(LogTemp, Display, TEXT("unspacetoolHTMLMenu Websocket Data: encoded: %i decoded:%i"), dataArrayFromSocket.Num(),decoded.Num());
							/*if (GEngine)
								GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow, FString::Printf(TEXT("unspacetoolHTMLMenu: 23: %s"), *recvMessage));*/
							if (mainlib != nullptr)
								mainlib->handleWebsocketData(recvMessage);
							decoded.Empty();
						}
					}
				}
			}
		}

		//UE_LOG(LogTemp, Display, TEXT("Websocket Disconnect"));

		run = false;

		//clean up socket
		if (clientSocket != nullptr) {
#if PLATFORM_IOS
			clientSocket->Close();
#endif
			clientSocket = nullptr;
		}
		thread = nullptr;

		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("END B"));*/
		return 0;
	}

public:
	void closeClientConnection() {
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("closeClientConnection"));*/
		if (run) {
			run = false;

		}
	}

	bool isRun() {
		return run;
	}


private:
	FSocket* clientSocket = nullptr;
	FRunnableThread* thread = nullptr;

	bool	needHandshake = true;
	bool	run = true;
	UunspacetoolHTMLMenuMainBPLibrary* mainlib = nullptr;

	FString generateHandshakeData(FString clientKey) {
		FString handshakeData = "HTTP/1.1 101 \r\nUpgrade: websocket\r\nConnection: upgrade\r\nSec-WebSocket-Accept: ";
		clientKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		FSHAHash Hash;
		FTCHARToUTF8 Convert(*clientKey);
		FSHA1::HashBuffer((uint8*)((ANSICHAR*)Convert.Get()), Convert.Length(), Hash.Hash);
		TArray<uint8> sha1EncodedBytes;
		sha1EncodedBytes.Append(Hash.Hash, 20);
		handshakeData += FBase64::Encode(sha1EncodedBytes) + "\r\n\r\n";
		return handshakeData;


		/*	FTCHARToUTF8 Convert(*clientKey);
			FSHA1::HashBuffer((uint8*)((ANSICHAR*)Convert.Get()), Convert.Length(), outbuffer.GetData());*/
	}
};


class FunspacetoolHTMLMenuWebsocketServerThread : public FRunnable {

public:

	FunspacetoolHTMLMenuWebsocketServerThread(UunspacetoolHTMLMenuMainBPLibrary* mainlibP) :
		mainlib(mainlibP) {
		FString threadName = "FunspacetoolHTMLMenuWebsocketServerThread_" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override {
		FIPv4Endpoint boundEndpoint;

		while (socketSubSystem != nullptr && websocketPortMin <= websocketPortMax) {

			if (serverStarted == false) {

				if (listenerSocket != nullptr) {
					listenerSocket->Close();
					listenerSocket = nullptr;
					FPlatformProcess::Sleep(0.2);
				}

				boundEndpoint = FIPv4Endpoint(FIPv4Address(127, 0, 0, 1), websocketPortMin);
				listenerSocket = socketSubSystem->CreateSocket(NAME_Stream, *FString("UunspacetoolHTMLMenuWebsocketServer"), boundEndpoint.ToInternetAddrIPV4()->GetProtocolType());

				if (listenerSocket != nullptr) {

					if (!listenerSocket->Bind(*boundEndpoint.ToInternetAddrIPV4())) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
							websocketPortMin++;
							FPlatformProcess::Sleep(0.2);
							continue;
						}
					}


					if (!listenerSocket->Listen(8)) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
						}
					}
				}

				if (listenerSocket == nullptr) {
					websocketPortMin++;
					FPlatformProcess::Sleep(0.2);
					continue;
				}

				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Cyan, TEXT("Server Started on:") + FString::FromInt(websocketPortMin));*/

				serverStarted = true;
			}

			if (websocketClientThread != nullptr) {
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Close client"));*/
				websocketClientThread->closeClientConnection();
				websocketClientThread = nullptr;
			}

			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("After Close client"))*/;

			mainlib->callJavascript("openWebsocket(" + FString::FromInt(websocketPortMin) + ");");
			int32 connectionAttempts = 20;
			while (listenerSocket != nullptr && connected == false) {
				connectionAttempts--;
				bool pending;
				listenerSocket->WaitForPendingConnection(pending, FTimespan::FromSeconds(1));
				//listenerSocket->HasPendingConnection(pending);
				if (pending) {
					TSharedRef<FInternetAddr> remoteAddress = socketSubSystem->CreateInternetAddr();
					FSocket* clientSocket = listenerSocket->Accept(*remoteAddress, FGuid::NewGuid().ToString());
					/*if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Blue, TEXT("New websocketClient Thread"));*/
					websocketClientThread = new FunspacetoolHTMLMenuWebsocketClientThread(clientSocket, mainlib);
					connected = true;
					/*if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("Client connected"));*/
					break;
				}
				else {
					mainlib->callJavascript("openWebsocket(" + FString::FromInt(websocketPortMin) + ");");
				}
				if (connectionAttempts < 0) {
					break;
				}
			}
	


			pauseThread(true);
			//workaround. suspend do not work on all platforms. lets sleep
			while (paused) {
				FPlatformProcess::Sleep(0.01);
			}

		}

		return 0;
	}

public:

	void pauseThread(bool pause) {
		paused = pause;
		thread->Suspend(pause);
	}

	void allowNewConnection() {
		if (listenerSocket != nullptr) {
		/*	if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, FString("END A ") + FString::FromInt(listenerSocket->GetConnectionState())+" "+ FString::FromInt(socketSubSystem->GetLastErrorCode()));*/

			if (websocketClientThread != nullptr) {
				if (websocketClientThread->isRun() == false) {
					serverStarted = false;
				}
			}

		}
		else {
			serverStarted = false;
		}
		connected = false;
		pauseThread(false);
	}

	void test() {
		listenerSocket->Close();
		if (websocketClientThread != nullptr) {
			websocketClientThread->closeClientConnection();
		}
	}


private:
	FRunnableThread* thread = nullptr;
	FSocket* listenerSocket = nullptr;
	bool connected = false;
	bool serverStarted = false;
	bool paused = false;
	UunspacetoolHTMLMenuMainBPLibrary* mainlib = nullptr;

	TArray<int32> ipArray;
	int32 websocketPortMin = 49800;
	int32 websocketPortMax = 50000;
	
	FunspacetoolHTMLMenuWebsocketClientThread* websocketClientThread = nullptr;
	ISocketSubsystem* socketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
};
