// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class unspace_tool : ModuleRules
{
	public unspace_tool(ReadOnlyTargetRules Target) : base(Target)
	{
		 PrecompileForTargets = PrecompileTargetsType.Any;
         PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("unspace_tool");

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "LiveLinkInterface", "CinematicCamera", "DisplayCluster","LiveLink",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
                "Projects",
                "Slate",
				"SlateCore",
                "ToolMenus",
                "UMG",
                "UMGEditor",
				"Blutility",
				"AssetTools",
                "UnrealEd",
				"RawMesh",
                "AssetRegistry",
                "RawMesh",
				"StaticMeshEditor",
                "StaticMeshDescription",
                "MeshDescription",
				"MeshConversion",
                "EditorScriptingUtilities",
                "ProceduralMeshComponent",
                "KismetCompiler",
                "DisplayCluster",
                "DisplayClusterConfiguration",
                "DisplayClusterConfigurator",
                "OpenColorIO",
                "ContentBrowserData",
                "CinematicCamera",
                "OpenColorIO", 
                "EditorStyle",
                
                "LiveLinkComponents",
                "LiveLink",
                "LiveLinkInterface",
                "LiveLinkAnimationCore",
                "LiveLinkFreeD",
                
                "OptitrackLiveLink",
                "OptitrackNatnet",
                "OptitrackNatnetEditor",

                "SwitchboardEditor",
                "HTTP",
                "HTTPServer",
                "Json",
                "JsonUtilities",
                
                //"DisplayClusterConfigurator"
				// ... add private dependencies that you statically link with here ...	
			}
			);

        // TODO: Should not be including private headers from a different module
        PrivateIncludePaths.AddRange(
            new string[] {
                Path.Combine(GetModuleDirectory("DisplayClusterConfigurator"), "Private"), // For DisplayClusterConfiguratorPropertyUtils.h
                Path.Combine(GetModuleDirectory("LiveLinkFreeD"), "Private")
			});


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
