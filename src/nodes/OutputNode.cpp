#include "OutputNode.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

// OutputNode class inherits from Node
OutputNode::OutputNode() : Node("Output Node") {
    savePath = "output.jpg";
}

// Destructor for OutputNode
void OutputNode::process() {
    if (inputs.empty() || inputs[0]->output.empty()) {
        output.release();
        return;
    }

    output = inputs[0]->output.clone(); // Clone the input image to output

    if (saveRequested) {
        if (cv::imwrite(savePath, output)) {
            std::cout << "Image saved to: " << savePath << std::endl;
        } else {
            std::cerr << "Failed to save image to: " << savePath << std::endl;
        } // Attempt to save the image
        saveRequested = false; // Reset the save request flag
    }
}

// Render the UI for the OutputNode
void OutputNode::renderUI() {
    ImGui::Text("Output Node");

    char buffer[512];
    strncpy(buffer, savePath.c_str(), sizeof(buffer)); // Copy save path to buffer
    if (ImGui::InputText("Save Path", buffer, sizeof(buffer))) {
        savePath = std::string(buffer);
    } // Input text for save path

    if (ImGui::Button("Save Image")) {
        saveRequested = true;
        isDirty = true;
    } // Button to request saving the image
}