#include "check_error.h"
#include<glad/glad.h>

static const char* GLErrorToString(GLenum e)
{
    switch (e)
    {
    case GL_NO_ERROR: return "GL_NO_ERROR";
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        // GL_STACK_UNDERFLOW/OVERFLOW 在 core profile 里可能没了，但有些实现还会报
#ifdef GL_STACK_UNDERFLOW
    case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
#endif
#ifdef GL_STACK_OVERFLOW
    case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
#endif
    default: return "UNKNOWN_GL_ERROR";
    }
}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR) {}
}

bool GLLogCall(const char* function, const char* file, int line)
{
    bool ok = true;
    for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError())
    {
        std::cerr << "[OpenGL Error] " << GLErrorToString(err)
            << " (0x" << std::hex << err << std::dec << ")\n"
            << "  at " << function << "\n"
            << "  " << file << ":" << line << "\n";
        ok = false;
    }
    return ok;
}
