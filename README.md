# Photo Dukaan – Qt Image Editor

**Photo Dukaan** is a C++ GUI application built with the Qt framework that allows users to apply various visual filters and effects to images. It features a user-friendly interface for browsing, editing, and saving images with real-time previews of applied filters.

---

## 🚀 Features

- 📁 **Browse and Load Images**
- 🖌️ **Apply Filters:**
  - Grayscale
  - Sepia
  - Invert Colors
  - Blur
  - Flip 180°
  - Rotate 90°
  - Pixelate
  - Green Tint
  - Heat Map
  - Posterize
  - Edge Detection
- 💾 **Save Edited Images**
- 🔄 **Reset to Original Image**
- 🎨 Dynamic color animation for the title

---

## 🛠️ Technologies Used

- **C++**
- **Qt Framework** (Widgets, GUI, Core, Multimedia)
- `QApplication`, `QWidget`, `QPushButton`, `QImage`, `QPixmap`, `QFileDialog`, `QTimer`, and more

---

## 🧩 How It Works

1. **Load an Image** using the "BROWSE" button.
2. **Apply a Filter** by clicking any of the filter buttons.
3. **View Real-Time Preview** of the modified image.
4. **Reset or Save** the image using the corresponding buttons.

---

## 🧪 Build and Run Instructions

### 🧰 Prerequisites

- [Qt 5.x or Qt 6.x](https://www.qt.io/download)
- A C++ compiler (GCC/Clang/MSVC)
- Qt Creator (recommended)

### ⚙️ Compilation Steps

#### Option 1: Using Qt Creator (recommended)

1. Open `main.cpp` in **Qt Creator**.
2. Configure your Kit (compiler + Qt version).
3. Click **Run** ▶️ to build and launch the app.

#### Option 2: Manual Compilation (with qmake or CMake)

```bash
qmake -project
qmake
make
./<executable_name>
