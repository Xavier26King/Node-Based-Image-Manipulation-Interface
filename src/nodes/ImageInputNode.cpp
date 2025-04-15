#include "ImageInputNode.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <iostream>

ImageInputNode::ImageInputNode()
    : Node("Image Input"), filePath("C:/Users/Hp/OneDrive/Documents/Pictures/Camera Roll/WIN_20211225_11_19_28_Pro.jpg") {}

// Destructor for ImageInputNode
// Destructor is empty as there are no dynamically allocated resources to clean up
void ImageInputNode::process() {
    if (!isDirty) return;

    output = cv::imread(filePath);
    if (output.empty()) {
        std::cerr << "Failed to load image at: " << filePath << std::endl;
    } else {
        std::cout << "Loaded image from: " << filePath << std::endl;
    }

    isDirty = false;  // ✅ CLEAR the dirty flag after loading
}

// Render the UI for the ImageInputNode
void ImageInputNode::renderUI() {
    ImGui::Text("Image Input Node");

    // Static buffer + one-time initialization flag
    static char buffer[512] = "";
    static bool initialized = false;

    // Set initial buffer only once
    if (!initialized) {
        strcpy_s(buffer, filePath.c_str());
        initialized = true;
    }

    // Apply new path only when Enter is pressed
    if (ImGui::InputText("Image Path", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        filePath = std::string(buffer);
    }

    // Manual reload also applies new path and marks as dirty
    if (ImGui::Button("Reload")) {
        filePath = std::string(buffer);
        isDirty = true;
    }
}

