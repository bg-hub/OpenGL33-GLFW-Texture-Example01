#include "ShaderProgram.hpp"
#include "OpenGLErrorHandler.hpp"
#include <cassert>
#include <iostream>


ShaderProgram::ShaderProgram() {
    
}

/*
 *  Der Destruktor gibt beide Shader und das Shaderprogramm frei.
 */
ShaderProgram::~ShaderProgram() {
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
}


void ShaderProgram::init_shader_interface() {
    init_shaders();    
    
    program = glCreateProgram();
    OpenGLErrorHandler::checkAndReportErrors("glCreateProgram");
    
    glAttachShader(program, vertexShader);
    OpenGLErrorHandler::checkAndReportErrors("glAttachShader");
    glAttachShader(program, fragmentShader);
    OpenGLErrorHandler::checkAndReportErrors("glAttachShader");
    glLinkProgram(program);
    OpenGLErrorHandler::checkAndReportErrors("glLinkProgram");
    //std::cout << "shader programm linked" << std::endl;
    glUseProgram(program);
}


/*
 *  Die Methode veranlasst das Einlesen und die Compilierung
 *  beider Shader.
 */
bool ShaderProgram::init_shaders() {
    bool res1, res2;
    res1 = loadAndCompileShader("./res/VertexShader.glsl",
                                vertexShader,
                                GL_VERTEX_SHADER);
    std::cout << "Compilation of Vertex shader: " << res1 << std::endl;
    res2 = loadAndCompileShader("./res/FragmentShader.glsl",
                                fragmentShader,
                                GL_FRAGMENT_SHADER);
    std::cout << "Compilation of Fragment shader: " << res2 << std::endl;
    return res1 && res2;
}

/*
 *
 */
bool ShaderProgram::loadAndCompileShader
           (const char *fileName,
            GLuint &shaderId,
            GLenum shaderType) {
    bool res = true;
    FILE* f = fopen(fileName, "rb");
    if (f == NULL) {
        return false;
    }
    //  Die Größe der zu lesenden Datei bestimmen:
    int success = fseek(f, 0, SEEK_END);
    assert(success == 0);
    long int fSize = ftell(f);           //  aktuelle Dateiposition.
                                         //  Da die Datei auf ihr Ende
                                         //  positioniert wurde, ist dies
                                         //  die Dateigröße.
    success = fseek(f ,0, SEEK_SET);     //  Datei auf ihren Anfang setzen
    assert(success == 0);
    {
        GLchar codeString[fSize + 1];
        size_t bytes_read = fread(codeString, 1, fSize, f);
        assert(fSize == (long int) bytes_read);
        codeString[fSize] = 0; //null terminate it!
        fclose(f);    
        GLchar* refString = &codeString[0];
        GLint compileStatus;
        std::cout << codeString << std::endl;
        std::cout << " ---------------------------------------------" << std::endl;
        shaderId  = glCreateShader(shaderType);
        std::cout << "shader created: " << shaderId << std::endl;
          OpenGLErrorHandler::checkAndReportErrors("glCreateShader(GL_VERTEX_SHADER)");
        glShaderSource(shaderId, 1, (const GLchar**)&refString, 0);
          OpenGLErrorHandler::checkAndReportErrors("glSgaderSource(GL_VERTEX_SHADER)");
        glCompileShader(shaderId);
          OpenGLErrorHandler::checkAndReportErrors("glCompileShader(GL_VERTEX_SHADER)");

        std::cout << "access compile status: ";
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
        std::cout << (compileStatus ? "OK" : "FAILURE") << std::endl;
         OpenGLErrorHandler::checkAndReportErrors("glGetShaderiv");
        if (!compileStatus) {  //  ausführliches Fehlerprotokoll
            res = false;
            std::cout << "compile status: " << compileStatus << std::endl;
            GLint infoLength = 0;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);
            if (infoLength > 1) {
                GLchar infoBuffer[infoLength + 10];
                glGetShaderInfoLog(shaderId, infoLength, NULL, infoBuffer);
                printf("shader:\n%s\n", infoBuffer);
            }
            glDeleteShader(shaderId);
            std::cout << "compilation failure in shader" << std::endl;
        }
    }
    std::cout << "done " << std::endl;
    return res;
}
