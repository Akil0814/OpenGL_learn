#递归本文件夹所有的.cpp文件
file(GLOB_RECURSE APP ./ *.cpp)
add_library(app ${APP})