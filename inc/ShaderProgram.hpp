#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_


#include "CustomizedGL.hpp"


/*
 *  Eine Instanz der Klasse  ShaderProgram leistet
 *  die Herstellung und die Freigabe eines Shaderprogramms
 *  für OpenGL 3.3.
 */
class ShaderProgram {

  public:
    ShaderProgram();
    ~ShaderProgram();

    void init_shader_interface();
    GLuint program;

    GLuint verbose;
    GLuint vertexShader;
    GLuint fragmentShader;

  private:
    bool init_shaders();
    bool loadAndCompileShader(const char *fileName,
                              GLuint &shaderId,
                              GLenum shaderType);
};


#endif
