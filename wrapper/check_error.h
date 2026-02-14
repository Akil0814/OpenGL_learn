#pragma once
#include<iostream>
#include<GLFW/glfw3.h>

#ifdef DEBUG
#define ASSERT(func) if(!(func)) __debugbreak()
#define GL_CALL(func) GLClearError();\
    func;\
    ASSERT(GLLogCall(#func,__FILE__,__LINE__))
#else
#define GL_CALL(func) func;
#endif


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);