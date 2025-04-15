// src/nodes/BrightnessContrastNode.h
#pragma once
#include "../core/Node.h"

// BrightnessContrastNode class inherits from Node
// This class represents a node that applies brightness and contrast adjustments to an image
class BrightnessContrastNode : public Node {
public:
    BrightnessContrastNode();

    float brightness; // -100 to 100
    float contrast;   // 0.0 to 3.0

    void process() override;
    void renderUI() override;
};
