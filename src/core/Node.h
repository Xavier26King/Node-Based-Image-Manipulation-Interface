#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class Node {
public:
    Node(const std::string& name) : name(name), isDirty(true) {}
    virtual ~Node() = default;

    std::string name;
    std::vector<Node*> inputs;
    cv::Mat output;

    bool isDirty;

    virtual void process() = 0;
    virtual void renderUI() = 0;
};