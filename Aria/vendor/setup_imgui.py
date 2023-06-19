"""
Build script for ImGui
"""

import os
import shutil

files = [
    "imconfig.h",
    "imgui_demo.cpp",
    "imgui_draw.cpp",
    "imgui_internal.h",
    "imgui_tables.cpp",
    "imgui_widgets.cpp",
    "imgui.cpp",
    "imgui.h",
    "imstb_rectpack.h",
    "imstb_textedit.h",
    "imstb_truetype.h",
    os.path.join("backends", "imgui_impl_glfw.h"),
    os.path.join("backends", "imgui_impl_glfw.cpp"),
    os.path.join("backends", "imgui_impl_opengl3_loader.h"),
    os.path.join("backends", "imgui_impl_opengl3.cpp"),
    os.path.join("backends", "imgui_impl_opengl3.h"),
    os.path.join("backends", "imgui_impl_vulkan.cpp"),
    os.path.join("backends", "imgui_impl_vulkan.h"),
]

SRC_DIR = "imgui"
DEST_DIR = "AriaImgui"
CMAKE_FILENAME = "AriaImguiCMakeLists.txt"
current_dir = os.path.abspath(os.path.dirname(__file__))
src = os.path.join(current_dir, SRC_DIR)
dest = os.path.join(current_dir, DEST_DIR)

if not os.path.exists(dest):
    os.mkdir(dest)

for file in files:
    if not os.path.exists(os.path.join(src, file)):
        print("Cannot find file: " + file)
    else:
        print("Copying file: " + file)
        new_dest = os.path.join(dest, file)
        t = os.path.basename(new_dest)
        parent = os.path.basename(os.path.dirname(new_dest))
        if parent != DEST_DIR:
            # from relative path, remove parent directory
            shutil.copyfile(
                os.path.join(src, file), os.path.join(dest, os.path.basename(file))
            )
        else:
            shutil.copyfile(os.path.join(src, file), os.path.join(dest, file))

cmake_filepath = os.path.join(current_dir, CMAKE_FILENAME)
if not os.path.exists(cmake_filepath):
    print("Cannot find AriaImguiCMakeLists.txt")
else:
    shutil.copyfile(cmake_filepath, os.path.join(dest, "CMakeLists.txt"))
