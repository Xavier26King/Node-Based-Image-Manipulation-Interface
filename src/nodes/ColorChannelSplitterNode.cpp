#include "ColorChannelSplitterNode.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <glad/glad.h>

ColorChannelSplitterNode::ColorChannelSplitterNode()
    : Node("Color Splitter"), showAlpha(false) {}

void ColorChannelSplitterNode::process() {
    if (inputs.empty() || !inputs[0]->output.data) {
        std::cerr << "ColorChannelSplitterNode: No valid input.\n";
        return;
    }

    if (!isDirty) return;

    cv::Mat input = inputs[0]->output;
    std::vector<cv::Mat> splitChannels;
    cv::split(input, splitChannels);

    int numChannels = static_cast<int>(splitChannels.size());

    for (int i = 0; i < 4; ++i) {
        if (i < numChannels) {
            channels[i] = splitChannels[i];
        } else {
            channels[i] = cv::Mat(); // empty
        }
    }

    isDirty = false;
}

void ColorChannelSplitterNode::renderUI() {
    ImGui::Text("Color Channel Splitter");

    ImGui::Checkbox("Show Alpha Channel", &showAlpha);

    const char* labels[4] = { "Red", "Green", "Blue", "Alpha" };
    for (int i = 0; i < (showAlpha ? 4 : 3); ++i) {
        if (channels[i].empty()) continue;

        ImGui::Text("%s Channel", labels[i]);

        // Normalize and display grayscale preview
        cv::Mat preview;
        cv::normalize(channels[i], preview, 0, 255, cv::NORM_MINMAX);
        preview.convertTo(preview, CV_8UC1);

        // Convert to RGBA (OpenGL expects 4 channels)
        cv::Mat rgba;
        cv::cvtColor(preview, rgba, cv::COLOR_GRAY2RGBA);

        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba.cols, rgba.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        ImGui::Image((ImTextureID)(intptr_t)texID, ImVec2(100, 100));
        glDeleteTextures(1, &texID);
    }

    isDirty = true; // Always rerender previews
}
