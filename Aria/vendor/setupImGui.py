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

    "backends\\imgui_impl_glfw.cpp",
    "backends\\imgui_impl_glfw.h",
    "backends\\imgui_impl_opengl3_loader.h",
    "backends\\imgui_impl_opengl3.cpp",
    "backends\\imgui_impl_opengl3.h",
    "backends\\imgui_impl_vulkan.cpp",
    "backends\\imgui_impl_vulkan.h",
]

src_dir = "imgui"
dest_dir = "AriaImgui"
cmake_filename = "AriaImguiCMakeLists.txt"
current_dir = os.path.abspath(os.path.dirname(__file__))
src = os.path.join(current_dir, src_dir)
dest = os.path.join(current_dir, dest_dir)

if not os.path.exists(dest):
    os.mkdir(dest)

for file in files:
    if not os.path.exists(os.path.join(src, file)):
        print("Cannot find file: " + file)
    else:
        print("Copying file: " + file)
        new_dest = os.path.join(dest, file)
        t = os.path.basename(new_dest)
        # get parent directory
        parent = os.path.basename(os.path.dirname(new_dest))
        if parent != dest_dir:
            # from relative path, remove parent directory
            shutil.copyfile(os.path.join(src, file),
                            os.path.join(dest, os.path.basename(file)))
        else:
            shutil.copyfile(os.path.join(src, file),  os.path.join(dest, file))

cmake_filepath = os.path.join(current_dir, cmake_filename)
if not os.path.exists(cmake_filepath):
    print("Canot find AriaImguiCMakeLists.txt")
else:
    # rename it to just CMakeLists.txt and copy it
    shutil.copyfile(cmake_filepath, os.path.join(dest, "CMakeLists.txt"))
