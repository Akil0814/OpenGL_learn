#pragma once
#include <csignal>
#include <iostream>


#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

#if !defined(NDEBUG) || defined(_DEBUG)
#define ASSERT(expr) do { if(!(expr)) DEBUG_BREAK(); } while(0)

#define GL_CALL(call) do {               \
        GLClearError();                      \
        (call);                              \
        ASSERT(GLLogCall(#call, __FILE__, __LINE__)); \
    } while(0)
#else
#define ASSERT(expr) do { (void)sizeof(expr); } while(0)
#define GL_CALL(call) do { (call); } while(0)
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
