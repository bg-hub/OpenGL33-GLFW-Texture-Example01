#include "OpenGLErrorHandler.hpp"

#include <GL/gl.h>
#include <iostream>

using namespace std;



bool OpenGLErrorHandler::checkAndReportErrors
             (char const *className,
              char const *fnName,
              int lineNro,
              char const *message)
{
    GLenum error;
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << message << ": ";
        switch (error) {
            case GL_INVALID_VALUE:
              std::cout << "GL_INVALID_VALUE";
            break;
            case GL_INVALID_OPERATION:
              std::cout << "GL_INVALID_OPERATION";
            break;
            case GL_INVALID_ENUM:
              std::cout << "GL_INVALID_ENUM";
            break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
              std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
            case GL_OUT_OF_MEMORY:
              std::cout << "GL_OUT_OF_MEMORY";
            break;
            default:
              std::cout << "unknown error: " << error;
            break;
        }
        std::cout << std::endl;
    }
    return error == GL_NO_ERROR;
}


bool OpenGLErrorHandler::checkAndReportErrors(char const *message)
{
    GLenum error;
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << message << ": ";
        switch (error) {
            case GL_INVALID_VALUE:
              std::cout << "GL_INVALID_VALUE";
            break;
            case GL_INVALID_OPERATION:
              std::cout << "GL_INVALID_OPERATION";
            break;
            case GL_INVALID_ENUM:
              std::cout << "GL_INVALID_ENUM";
            break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
              std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
            case GL_OUT_OF_MEMORY:
              std::cout << "GL_OUT_OF_MEMORY";
            break;
            default:
              std::cout << "unknown error: " << error;
            break;
        }
        std::cout << std::endl;
    }
    return error == GL_NO_ERROR;
}
