#pragma once
#include "../core/Node.h"
#include <array>

class ColorChannelSplitterNode : public Node {
public:
    ColorChannelSplitterNode();

    std::array<cv::Mat, 4> channels; // R, G, B, A (or ignored)
    bool showAlpha;

    void process() override;
    void renderUI() override;
};
