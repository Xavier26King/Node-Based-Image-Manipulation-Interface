#pragma once
#include "../core/Node.h"
#include <array>

// ColorChannelSplitterNode class inherits from Node
// This class represents a node that splits an image into its color channels (R, G, B, A)
class ColorChannelSplitterNode : public Node {
public:
    ColorChannelSplitterNode();

    std::array<cv::Mat, 4> channels; // R, G, B, A (or ignored)
    bool showAlpha;

    void process() override;
    void renderUI() override;
};
