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

// GLFW error callback
// This function is called when GLFW encounters an error
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    // Setup GLFW window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return -1;

    const char* glsl_version = "#version 130"; // GLSL version
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Node-Based Image Editor", nullptr, nullptr); // Create window
    glfwMakeContextCurrent(window); // Make the window's context current
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup ImGui style
    ed::Config config;
    ed::EditorContext* editorContext = ed::CreateEditor(&config);
    ed::SetCurrentEditor(editorContext);

    // Setup OpenGL
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
        glfwPollEvents(); // Poll for and process events
        ImGui_ImplOpenGL3_NewFrame(); // Start the ImGui frame
        ImGui_ImplGlfw_NewFrame(); 
        ImGui::NewFrame(); 
        
        // [1] Create a menu bar with File menu
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open")) { std::cout << "Open clicked\n"; }
                if (ImGui::MenuItem("Save")) { std::cout << "Save clicked\n"; }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ed::Begin("Canvas");
        // Render the graph here
        ed::BeginNode(1);
       
        inputNode->renderUI();
        ed::EndNode();

        // [2] Render Brightness Node here
        ed::BeginNode(2);
        brightnessNode->renderUI();
        ed::EndNode();
        
        // [3] Render Splitter Node here
        ed::BeginNode(3);
        splitterNode->renderUI();
        ed::EndNode();
        
        // [4] Render Output Node here
        ed::BeginNode(4);
        outputNode->renderUI();
        ed::EndNode();

        graph.evaluate();
        
        // Render connections
        if (!brightnessNode->output.empty()) {
            if (textureID == 0) glGenTextures(1, &textureID); // Generate texture ID
            cv::Mat rgba; // Create a Mat to hold the RGBA image
            cv::cvtColor(brightnessNode->output, rgba, cv::COLOR_BGR2RGBA); // Convert BGR to RGBA
            // Bind the texture
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba.cols, rgba.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

            // Show image
            ImGui::Begin("Output Preview"); // Create a new window for the image preview
            ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(outputNode->output.cols, outputNode->output.rows)); // Display the image
            ImGui::End(); // End of image preview window
        }


        ImGui::Render(); // Render ImGui
        int display_w, display_h; // Get window size
        glfwGetFramebufferSize(window, &display_w, &display_h); // Get framebuffer size
        glViewport(0, 0, display_w, display_h); // Set viewport
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Render ImGui
        glfwSwapBuffers(window); // Swap buffers
    }

    if (textureID) glDeleteTextures(1, &textureID); // Clean up texture
    ed::DestroyEditor(editorContext); // Destroy editor context
    ImGui_ImplOpenGL3_Shutdown(); // Shutdown OpenGL3
    ImGui_ImplGlfw_Shutdown(); // Shutdown GLFW
    ImGui::DestroyContext(); // Destroy ImGui context
    glfwDestroyWindow(window); // Destroy window
    glfwTerminate();

    // Clean up nodes
    delete inputNode;
    delete brightnessNode;
    delete splitterNode;
    delete outputNode;

    return 0;
}