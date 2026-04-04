cmake_minimum_required(VERSION 3.12)

#项目名称
project(OpenGL)

#c++版本
set(CMAKE_CXX_STANDARD 17)

#加入include文件
include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib
)

#把需要拷贝的资源路径都放到ASSETS里
file(GLOB ASSETS "./assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

#将文件夹纳入编译
add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(GLframework)
add_subdirectory(imgui)

#搜索所有的cpp， 加入SRCS变量中
#aux_source_directory(. SRCS)

#可执行文件名称/ 工程文件
add_executable(OpenGL_study "main.cpp" "glad.c" "imgui/imconfig.h" "imgui/imgui.cpp" "imgui/imgui.h" "imgui/imgui_draw.cpp" "imgui/imgui_impl_glfw.cpp" "imgui/imgui_impl_glfw.h" "imgui/imgui_impl_opengl3.cpp" "imgui/imgui_impl_opengl3.h" "imgui/imgui_impl_opengl3_loader.h" "imgui/imgui_internal.h" "imgui/imgui_tables.cpp" "imgui/imgui_widgets.cpp" "imgui/imstb_rectpack.h" "imgui/imstb_textedit.h" "imgui/imstb_truetype.h")

#将funcs.lib链接入softRender
target_link_libraries(OpenGL_study glfw3.lib assimp-vc143-mtd.lib zlibstaticd.lib wrapper app fw imguilib)