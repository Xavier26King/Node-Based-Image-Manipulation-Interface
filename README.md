# Node-Based-Image-Manipulation-Interface

```markdown
# 🧠 Node-Based Image Manipulation Interface

A desktop application built in C++ that allows users to manipulate images through a visual, node-based interface — similar to tools like [Substance Designer](https://www.substance3d.com/documentation/sddoc/what-is-substance-designer-172823118.html).

Users can load an image, connect processing nodes, tweak parameters in real time, and export the final output.

---

## 🚀 Features (Phases Overview)

### ✅ Phase 1: Project Setup and Core UI
- Built using **C++17**, **OpenCV**, **Dear ImGui**, **imgui-node-editor**, and **OpenGL**
- GLFW window and rendering loop
- Canvas with draggable nodes
- Real-time OpenGL image preview
- File menu (Open, Save)

### ✅ Phase 2: Node System Core
- Abstract `Node` class with inputs/outputs
- `NodeGraph` for managing connections
- Support for:
  - Topological sorting of node graph
  - Dirty flag for caching image results
  - Dependency management and safe evaluation

### ✅ Phase 3: Graphical Node Editor
- Visual node editor with:
  - Nodes, pins, and connections
  - Real-time evaluation of changes
  - Resizable and interactive GUI
- Canvas powered by `imgui-node-editor`
- Integrated node rendering with ImGui

### ✅ Phase 4: Processing Nodes
Includes 4 categories of nodes:

#### 🧱 Basic Nodes
- **Image Input Node**: Load images and display metadata
- **Output Node**: Save final output with format options

#### 🎚️ Intermediate Nodes
- **Brightness/Contrast Node**: Sliders to adjust brightness (-100 to +100) and contrast (0 to 3) with reset buttons
- **Color Channel Splitter Node**: Splits RGB(A) image into grayscale previews of each channel

> More nodes like Blur, Thresholding, Edge Detection, Blend, Noise Generator, and Convolution Filter are planned for future phases.

---

## 🏗️ Architecture Overview

```
[ ImageInputNode ]
        ↓
[ BrightnessContrastNode ]
        ↓
[ ColorChannelSplitterNode ]
        ↓
[ OutputNode ]
```

Each node processes images using OpenCV and stores its result.  
Connections between nodes are handled by `NodeGraph`, ensuring correct evaluation order and avoiding cycles.

---

## 💻 Technologies Used

| Category       | Tools/Libs                              |
|----------------|------------------------------------------|
| Language       | C++17                                    |
| Image Processing | [OpenCV](https://opencv.org/)          |
| GUI            | [Dear ImGui](https://github.com/ocornut/imgui) |
| Node UI        | [imgui-node-editor](https://github.com/thedmd/imgui-node-editor) |
| Rendering      | OpenGL                                   |
| Windowing      | GLFW                                     |
| Build System   | CMake                                    |
| Dependency Mgmt| vcpkg                                    |

---

## 🧩 Installation Instructions

### 🔧 Prerequisites
- CMake ≥ 3.15
- Visual Studio 2019/2022 (on Windows)
- OpenGL-compatible GPU
- `vcpkg` installed and configured

### 📦 Install Dependencies
```bash
vcpkg install opencv imgui glfw glad
```

---

### 🏗️ Build Instructions (Windows Example)

```bash
cd node_editor
mkdir build
cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Users/<yourname>/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

Run the executable from `build/Debug/NodeEditor.exe`

---

## 🧪 Usage Guide

1. Launch the app
2. The default image will load
3. Drag nodes and edit parameters in real time
4. Click **Reload** to apply new image path
5. View preview thumbnails for:
   - Adjusted image
   - R/G/B/A grayscale splits
6. Export final output via Output Node

---

## 📂 Project Structure

```
node_editor/
├── CMakeLists.txt
├── src/
│   ├── core/
│   │   ├── Node.h / NodeGraph.h / NodeGraph.cpp
│   └── nodes/
│       ├── ImageInputNode.*
│       ├── BrightnessContrastNode.*
│       ├── ColorChannelSplitterNode.*
│       └── OutputNode.*
├── third_party/
│   ├── imgui/ (submodule)
│   ├── imgui-node-editor/
```

---

## 🛠️ Planned Nodes (Phase 5+)

- [ ] ✅ Gaussian Blur Node
- [ ] ✅ Adaptive Threshold Node
- [ ] ✅ Edge Detection (Sobel, Canny)
- [ ] ✅ Blend Node (Multiply, Screen, Overlay...)
- [ ] ✅ Noise Generator (Perlin, Simplex)
- [ ] ✅ Custom Convolution Kernel Node

---

## 🧠 Credits

- UI inspired by [imgui-node-editor](https://github.com/thedmd/imgui-node-editor)
- GUI framework: [Dear ImGui](https://github.com/ocornut/imgui)
- Image processing: [OpenCV](https://opencv.org/)

---

## 📜 License

This project is for educational and non-commercial use.  
You may fork or use parts under your own license with attribution.

---

## ✨ Demo

![Screenshot](./docs/screenshot.png)

---

## 🔗 Author

- 👨‍💻 Created by [Xavier Pandey](https://github.com/Xavier26King)
- 💬 DM on GitHub for collaboration or improvements!
```

---

### ✅ Next Steps

- Add this as a `README.md` in your root directory
- Optionally push with:

```bash
git add README.md
git commit -m "Added full project README with all phases"
git push
```

Let me know if you'd like help making a GitHub Pages site from this, or want badges, license, or release instructions!