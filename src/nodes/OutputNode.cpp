#include "OutputNode.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

OutputNode::OutputNode() : Node("Output Node") {
    savePath = "output.jpg";
}

void OutputNode::process() {
    if (inputs.empty() || inputs[0]->output.empty()) {
        output.release();
        return;
    }

    output = inputs[0]->output.clone();

    if (saveRequested) {
        if (cv::imwrite(savePath, output)) {
            std::cout << "Image saved to: " << savePath << std::endl;
        } else {
            std::cerr << "Failed to save image to: " << savePath << std::endl;
        }
        saveRequested = false;
    }
}

void OutputNode::renderUI() {
    ImGui::Text("Output Node");

    char buffer[512];
    strncpy(buffer, savePath.c_str(), sizeof(buffer));
    if (ImGui::InputText("Save Path", buffer, sizeof(buffer))) {
        savePath = std::string(buffer);
    }

    if (ImGui::Button("Save Image")) {
        saveRequested = true;
        isDirty = true;
    }
}