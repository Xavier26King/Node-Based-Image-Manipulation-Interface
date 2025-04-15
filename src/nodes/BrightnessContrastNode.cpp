// src/nodes/BrightnessContrastNode.cpp
#include "BrightnessContrastNode.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <iostream>

// Constructor for BrightnessContrastNode
// Initializes the node with a name and default values for brightness and contrast
BrightnessContrastNode::BrightnessContrastNode()
    : Node("Brightness/Contrast"), brightness(0.0f), contrast(1.0f) {}

// Destructor for BrightnessContrastNode
// Destructor is empty as there are no dynamically allocated resources to clean up
void BrightnessContrastNode::process() {
    if (inputs.empty() || !inputs[0]->output.data) {
        std::cerr << "BrightnessContrastNode: No valid input.\n";
        return;
    }

    if (!isDirty) return; // Only process if the node is dirty

    cv::Mat input = inputs[0]->output; // Get the input image from the first input node
    input.convertTo(output, -1, contrast, brightness);  // alpha = contrast, beta = brightness

    isDirty = false; // Mark the node as clean after processing
}

// Render the UI for the BrightnessContrastNode
// This function creates sliders for brightness and contrast adjustments
void BrightnessContrastNode::renderUI() {
    ImGui::Text("Brightness/Contrast Node");

    bool changed = false;

    ImGui::Text("Brightness");
    changed |= ImGui::SliderFloat("##BrightnessSlider", &brightness, -100.0f, 100.0f); // Brightness range is typically [-100, 100]
    // The range can be adjusted based on the expected input image type
    ImGui::SameLine();
    if (ImGui::Button("Reset##Brightness")) {
        brightness = 0.0f;
        changed = true;
    } // Reset button for brightness

    ImGui::Text("Contrast");
    changed |= ImGui::SliderFloat("##ContrastSlider", &contrast, 0.0f, 3.0f); // Contrast range is typically [0, 3]
    ImGui::SameLine();
    if (ImGui::Button("Reset##Contrast")) {
        contrast = 1.0f;
        changed = true;
    } // Reset button for contrast

    if (changed) {
        isDirty = true;
    }
}

