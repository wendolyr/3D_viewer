# 3D Viewer

A comprehensive 3D model visualization application built with **C++** and **Qt**. This software allows users to load, manipulate, and customize the rendering of 3D models from `.obj` files.

---

## 🚀 Features

### 📂 Model Loading & Parsing
-   Loads and parses 3D models from Wavefront `.obj` files.
-   Extracts vertices, edges, and faces for precise rendering.

### 🔄 Affine Transformations
-   Apply **move (translation)**, **rotate**, and **scale** transformations to the model in real-time.

### 🎨 Customizable Rendering
-   **Projection Types**: Switch between **central (perspective)** and **parallel (orthographic)** projections.
-   **Edge Display**: Customize edge **color**, **thickness**, and **style** (solid or dashed).
-   **Vertex Display**: Customize vertex **color**, **size**, and display mode (none 🔇, circle 🔵, or square 🟦).
-   **Background Color**: Change the viewport background color.

### 🖱️ Mouse Controls
-   **Rotation**: Left-click and drag.
-   **Panning**: Middle-click and drag. Hold `Shift` while dragging to move along the Z-axis.
-   **Zoom**: Mouse wheel scroll.

### 📸 Recording & Capture
-   **Screenshots**: Save the current stunning view as `BMP` or `JPEG` images.
-   **GIF Recording**: Record a 5-second GIF animation of the model view.

### 💾 State Persistence
-   The application automatically saves the last loaded model and all your display/transformation settings between sessions.

---

## 🏗️ Architecture & Patterns

The project is built following the **Model-View-Controller (MVC)** pattern, ensuring a clean separation of concerns.

-   **`Model` (`/model/`)**
    -   `FigureModel`: The core data structure holding vertices, edges, and transformation state.
    -   `FileManager`: Responsible for parsing `.obj` files and serializing/deserializing model and application state.
-   **`Controller` (`/controller/`)**
    -   `Facade`: Provides a simplified, unified interface to the complex Model layer (Facade pattern). It's the main entry point for the View.
-   **`View` (`/view/`)**
    -   `MainWidget`: The main window UI, built with Qt widgets.
    -   `OpenGLWidget`: A custom Qt widget handling all OpenGL rendering.
    -   `ShaderManager`: Manages the loading and compilation of GLSL shader programs.

### 🧩 Design Patterns Used:
-   **Strategy** (`strategy.h/cc`): Encapsulates affine transformation algorithms into interchangeable objects.
-   **Factory Method** (`factory.h`): Creates strategy objects.
-   **Facade** (`facade.h/cc`): Simplifies interaction with complex subsystems.
-   **Builder**: Used in UI code to simplify the construction of complex control groups.

---

## 🧮 Math & Rendering

-   **Transformations**: Affine transformations are applied using `4x4` matrices.
-   **Rotation**: Handled using **quaternions** to avoid gimbal lock.
-   **Rendering**: Utilizes **Modern OpenGL (4.1 Core Profile)** with custom shader programs:
    -   **Vertex Shaders**: Process vertex positions.
    -   **Geometry Shaders**: Generate thick lines and dashes on the GPU.
    -   **Fragment Shaders**: Calculate the final pixel color.

---

## 🔧 Build Instructions

### Prerequisites
-   A C++ Compiler with **C++20** support (GCC, Clang, MSVC)
-   **Qt** development libraries (version 6)
-   **CMake**
-   **FFmpeg** (required for GIF recording functionality)
-   MAC OS / LINUX

### Steps
1.  **Clone the repository**
    ```bash
    git clone git@github.com:wendolyr/3D_viewer.git
    cd 3D_Viewer
    ```

2.  **Compile the project**
    ```bash
    make install
    ```

3.  **Run the executable!** 🎉
    ```bash
    make run
    ```

---

## 👥 Development Team

This project was developed collaboratively by:
- **chovieti (School 21)**
- **flandera (School 21)**
