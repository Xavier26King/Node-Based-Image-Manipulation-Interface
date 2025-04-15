#include "ColorChannelSplitterNode.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <glad/glad.h>

// Constructor for ColorChannelSplitterNode
// Initializes the node with a name and default values for the channels
ColorChannelSplitterNode::ColorChannelSplitterNode()
    : Node("Color Splitter"), showAlpha(false) {}

// Destructor for ColorChannelSplitterNode
// Destructor is empty as there are no dynamically allocated resources to clean up
void ColorChannelSplitterNode::process() {
    if (inputs.empty() || !inputs[0]->output.data) {
        std::cerr << "ColorChannelSplitterNode: No valid input.\n";
        return;
    }

    if (!isDirty) return; // Only process if the node is dirty

    cv::Mat input = inputs[0]->output; // Get the input image from the first input node
    std::vector<cv::Mat> splitChannels; // Vector to hold the split channels
    cv::split(input, splitChannels); // Split the input image into its color channels

    int numChannels = static_cast<int>(splitChannels.size()); // Number of channels in the input image

    for (int i = 0; i < 4; ++i) {
        if (i < numChannels) {
            channels[i] = splitChannels[i];
        } else {
            channels[i] = cv::Mat(); // empty
        }
    } // Store the split channels in the channels vector

    isDirty = false;
}

// Render the UI for the ColorChannelSplitterNode
void ColorChannelSplitterNode::renderUI() {
    ImGui::Text("Color Channel Splitter"); // Display the node name

    ImGui::Checkbox("Show Alpha Channel", &showAlpha); // Checkbox to toggle alpha channel display

    const char* labels[4] = { "Red", "Green", "Blue", "Alpha" };
    for (int i = 0; i < (showAlpha ? 4 : 3); ++i) {
        if (channels[i].empty()) continue; // Skip empty channels

        ImGui::Text("%s Channel", labels[i]); // Display the channel name

        // Normalize and display grayscale preview
        cv::Mat preview;
        cv::normalize(channels[i], preview, 0, 255, cv::NORM_MINMAX); // Normalize the channel to 0-255 range
        preview.convertTo(preview, CV_8UC1); // Convert to 8-bit single channel

        // Convert to RGBA (OpenGL expects 4 channels)
        cv::Mat rgba;
        cv::cvtColor(preview, rgba, cv::COLOR_GRAY2RGBA); // Convert grayscale to RGBA

        // Create OpenGL texture and bind it
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
