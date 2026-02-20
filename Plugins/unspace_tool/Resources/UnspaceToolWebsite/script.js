
function selectTab(evt, tabName) {
  var i, tabcontent, tablinks;

  tabcontent = document.getElementsByClassName("content");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }

  tablinks = document.getElementsByClassName("tab");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }

  document.getElementById(tabName).style.display = "block";
  evt.currentTarget.className += " active";

  subNavbar = document.querySelector(".sub-navbar");
  if (tabName === "uBuild" || tabName === "uConfig") {
    subNavbar.style.display = "flex"; 
  } else {
    subNavbar.style.display = "none"; 
  }
}

function selectSubTab(evt, subTabName) {
  var i, subContent, subTabs;

  subTabs = document.getElementsByClassName("sub-tab");
  for (i = 0; i < subTabs.length; i++) {
    subTabs[i].className = subTabs[i].className.replace(" active", "");
  }

  document.getElementById(subTabName).style.display = "block";
  evt.currentTarget.className += " active";
}

document
  .getElementById("buildSettingsForm")
  .addEventListener("submit", function (event) {
    event.preventDefault();
    alert("Form submitted!");
    triggerUE4EventWithTag('BuildMeshForm', 'TestData');
  });

function disableFormSection(sectionId) {
  var section = document.getElementById(sectionId);
  if (!section) return; 

  var elements = section.querySelectorAll("input, button, select, textarea, a");
  elements.forEach(function (element) {
    element.classList.add("disabled-style");
    element.disabled = true; 
    if (element.tagName.toLowerCase() === "a") {
      element.classList.add("disabled-link"); 
      element.setAttribute("tabindex", "-1"); 
      element.style.pointerEvents = "none"; 
    }
  });

  var elements = section.querySelectorAll("label, h1, h2, p");
  elements.forEach(function (element) {
    element.classList.add("disabled-style"); 
    element.disabled = true; 
    if (element.tagName.toLowerCase() === "a") {
      element.classList.add("disabled-link"); 
      element.setAttribute("tabindex", "-1"); 
      element.style.pointerEvents = "none"; 
    }
  });

  toggleTextDisabled(sectionId, true);
}

function enableFormSection(sectionId) {
  var section = document.getElementById(sectionId);
  if (!section) return; 

  var elements = section.querySelectorAll("input, button, select, textarea, a");
  elements.forEach(function (element) {
    element.classList.remove("disabled-style"); 
    element.disabled = false; 
    if (element.tagName.toLowerCase() === "a") {
      element.classList.remove("disabled-link");
    }
  });
  toggleTextDisabled(sectionId, false);
}

function toggleTextDisabled(sectionId, disable) {
  var section = document.getElementById(sectionId);
  if (!section) return; 

  var textElements = section.querySelectorAll("label, p, h1, h2");
  textElements.forEach(function (element) {
    if (disable) {
      element.classList.add("disabled-text"); 
      element.style.opacity = 0.5; 
    } else {
      element.classList.remove("disabled-text"); 
      element.style.opacity = 1; 
    }
  });
}

function showCinecameraSettingsPopup() {
  var modal = document.getElementById("cineCameraModal");
  var modalclass = document.getElementsByClassName("modal");
  modalclass.style.display = "block";
  modal.style.display = "block";
}

function closeCineCameraSettingsPopup() {
  var modal = document.getElementById("cineCameraModal");
  modal.style.display = "none";
}

window.onclick = function (event) {
  var modal = document.getElementById("cineCameraModal");
  if (event.target == modal) {
    modal.style.display = "none";
  }
};
