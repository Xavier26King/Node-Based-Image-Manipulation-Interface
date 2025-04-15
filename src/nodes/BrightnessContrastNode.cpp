// src/nodes/BrightnessContrastNode.cpp
#include "BrightnessContrastNode.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <iostream>

BrightnessContrastNode::BrightnessContrastNode()
    : Node("Brightness/Contrast"), brightness(0.0f), contrast(1.0f) {}

void BrightnessContrastNode::process() {
    if (inputs.empty() || !inputs[0]->output.data) {
        std::cerr << "BrightnessContrastNode: No valid input.\n";
        return;
    }

    if (!isDirty) return;

    cv::Mat input = inputs[0]->output;
    input.convertTo(output, -1, contrast, brightness);  // alpha = contrast, beta = brightness

    isDirty = false;
}

void BrightnessContrastNode::renderUI() {
    ImGui::Text("Brightness/Contrast Node");

    bool changed = false;

    ImGui::Text("Brightness");
    changed |= ImGui::SliderFloat("##BrightnessSlider", &brightness, -100.0f, 100.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset##Brightness")) {
        brightness = 0.0f;
        changed = true;
    }

    ImGui::Text("Contrast");
    changed |= ImGui::SliderFloat("##ContrastSlider", &contrast, 0.0f, 3.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset##Contrast")) {
        contrast = 1.0f;
        changed = true;
    }

    if (changed) {
        isDirty = true;
    }
}

