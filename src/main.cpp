#define IMGUI_DEFINE_MATH_OPERATORS

#include <opencv2/opencv.hpp>
#include "imgui.h"
#include "E:/Projects/node_editor/third_party/imgui_backends/imgui_impl_glfw.h"
#include "E:/Projects/node_editor/third_party/imgui_backends/imgui_impl_openg13.h"
#include "E:/Projects/node_editor/third_party/imgui_node_editor/imgui_node_editor.h"
#include "E:/Projects/node_editor/third_party/imgui_node_editor/imgui_extra_math.h"
#include "E:/Projects/node_editor/third_party/imgui_node_editor/imgui_bezier_math.h"
#include "E:/Projects/node_editor/third_party/imgui_node_editor/imgui_canvas.h"
#include "E:/Projects/node_editor/third_party/imgui_node_editor/crude_json.h"
#include "E:/Projects/node_editor/third_party/imgui_node_editor/imgui_node_editor_internal.inl"
namespace ed = ax::NodeEditor;

#include <GLFW/glfw3.h>
#include <iostream>

#include "core/NodeGraph.h"
#include "nodes/ImageInputNode.h"
#include "nodes/OutputNode.h"
#include "nodes/BrightnessContrastNode.h"
#include "nodes/ColorChannelSplitterNode.h"


void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    // Setup GLFW window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return -1;

    const char* glsl_version = "#version 130";
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Node-Based Image Editor", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ed::Config config;
    ed::EditorContext* editorContext = ed::CreateEditor(&config);
    ed::SetCurrentEditor(editorContext);

    NodeGraph graph;
    ImageInputNode* inputNode = new ImageInputNode();
    graph.addNode(inputNode);

    BrightnessContrastNode* brightnessNode = new BrightnessContrastNode();
    graph.addNode(brightnessNode);

    ColorChannelSplitterNode* splitterNode = new ColorChannelSplitterNode();
    graph.addNode(splitterNode);

    OutputNode* outputNode = new OutputNode();
    graph.addNode(outputNode);
    // graph.connect(inputNode, outputNode);
    // Connect: input → brightness → splitter -> output
    graph.connect(inputNode, brightnessNode);       // image → brightness
    graph.connect(brightnessNode, splitterNode);    // brightness → splitter
    graph.connect(splitterNode, outputNode);        // splitter → output


    GLuint textureID = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open")) { std::cout << "Open clicked\n"; }
                if (ImGui::MenuItem("Save")) { std::cout << "Save clicked\n"; }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ed::Begin("Canvas");

        ed::BeginNode(1);
       
        inputNode->renderUI();
        ed::EndNode();

        // [2] Render Brightness Node here
        ed::BeginNode(2);
        brightnessNode->renderUI();
        ed::EndNode();

        ed::BeginNode(3);
        splitterNode->renderUI();
        ed::EndNode();

        ed::BeginNode(4);
        outputNode->renderUI();
        ed::EndNode();

        graph.evaluate();

        if (!brightnessNode->output.empty()) {
            if (textureID == 0) glGenTextures(1, &textureID);
            cv::Mat rgba;
            cv::cvtColor(brightnessNode->output, rgba, cv::COLOR_BGR2RGBA);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba.cols, rgba.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Show image
            ImGui::Begin("Output Preview");
            ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(outputNode->output.cols, outputNode->output.rows));
            ImGui::End();
        }

        // if (textureID != 0 && !outputNode->output.empty()) {
        //     ImGui::Begin("Output Preview");
        //     ImGui::Image((ImTextureID)(intptr_t)textureID,
        //                  ImVec2(outputNode->output.cols, outputNode->output.rows));
        //     ImGui::End();
        // }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    if (textureID) glDeleteTextures(1, &textureID);
    ed::DestroyEditor(editorContext);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    delete inputNode;
    delete brightnessNode;
    delete splitterNode;
    delete outputNode;

    return 0;
}