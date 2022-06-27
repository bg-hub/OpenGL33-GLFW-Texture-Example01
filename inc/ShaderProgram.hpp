#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_


#include "CustomizedGL.hpp"


/*
 *  Eine Instanz der Klasse  ShaderProgram leistet
 *  die Herstellung und die Freigabe eines Shaderprogramms
 *  für OpenGL 4.5.
 */
class ShaderProgram {

  public:
    ShaderProgram();
    ~ShaderProgram();

    void createShaderProgram();
    GLuint getProgramID();
    void setVerbose(bool v);

  private:
    GLuint verbose;
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    bool initShaders();
    bool loadAndCompileShader(const char *fileName,
                              GLuint &shaderId,
                              GLenum shaderType);
};


#endif
