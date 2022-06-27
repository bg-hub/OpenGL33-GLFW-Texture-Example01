#include "ShaderProgram.hpp"
#include "OpenGLErrorHandler.hpp"
#include <cassert>
#include <iostream>


ShaderProgram::ShaderProgram() :
    verbose(false)
{

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

GLuint ShaderProgram::getProgramID() {
    return program;
}

void ShaderProgram::setVerbose(bool v) {
    verbose = v;
}

/*
 *  Die Methode erzeugt das Shaderprogramm und stellt es für
 *  die Benutzung bereit.
 */
void ShaderProgram::createShaderProgram() {
    initShaders();    
    
    program = glCreateProgram();
    OpenGLErrorHandler::checkAndReportErrors("glCreateProgram");
    
    glAttachShader(program, vertexShader);
    OpenGLErrorHandler::checkAndReportErrors("glAttachShader");
    glAttachShader(program, fragmentShader);
    OpenGLErrorHandler::checkAndReportErrors("glAttachShader");
    glLinkProgram(program);
    OpenGLErrorHandler::checkAndReportErrors("glLinkProgram");
    if (verbose) {
        std::cout << "shader programm linked" << std::endl;
    }
    glUseProgram(program);
}


/*
 *  Die Methode veranlasst das Einlesen und die Compilierung
 *  beider Shader.
 */
bool ShaderProgram::initShaders() {
    bool res1, res2;
    res1 = loadAndCompileShader("./res/VertexShader.glsl",
                                vertexShader,
                                GL_VERTEX_SHADER);
    if (verbose) {
        std::cout << "Compilation of Vertex shader: " << res1 << std::endl;
    }
    res2 = loadAndCompileShader("./res/FragmentShader.glsl",
                                fragmentShader,
                                GL_FRAGMENT_SHADER);
    if (verbose) {
        std::cout << "Compilation of Fragment shader: " << res2 << std::endl;
    }
    return res1 && res2;
}

/*
 *  Die Methode liest den Quelltext eines Shaders aus der
 *  angegebenen Datei und compiliert ihn.
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
        if (verbose) {
            std::cout << codeString << std::endl;
            std::cout << " ---------------------------------------------" << std::endl;
        }
        shaderId  = glCreateShader(shaderType);
          OpenGLErrorHandler::checkAndReportErrors("glCreateShader(GL_VERTEX_SHADER)");
        glShaderSource(shaderId, 1, (const GLchar**)&refString, 0);
          OpenGLErrorHandler::checkAndReportErrors("glSgaderSource(GL_VERTEX_SHADER)");
        glCompileShader(shaderId);
          OpenGLErrorHandler::checkAndReportErrors("glCompileShader(GL_VERTEX_SHADER)");

        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
         OpenGLErrorHandler::checkAndReportErrors("glGetShaderiv");
        if (verbose) {
            std::cout << "Shader Id: " << shaderId 
                      << " compile status: "
                      << (compileStatus ? "OK" : "FAILURE") << std::endl;
        }
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
    return res;
}
