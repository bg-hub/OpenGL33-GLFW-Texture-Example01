#ifndef _SHADER_LIGHT_H_
#define _SHADER_LIGHT_H_

#include "CustomizedGL.hpp"
#include "LightProperties.hpp"

/*
 *  Eine Instanz speichert die Adressen der Konstanten
 *  des Shaderprogramms, die die Eigenschaften einer
 *  Lichtquelle bestimmen.
 *
 *  Eine gleichartige Struktur wird im Vertex Shader
 *  verwendet, um die Eigenschaften einer Lichtquelle
 *  zu beschreiben.
 *
 *  Eine Instanzmethode leistet die Übertragung der
 *  Werte an die GPU.
 *  Diese Klasse ist für einen Shader bestimmt, der
 *  Lichtquellen ohne Lichtabschwächung bereitstellt.
 *  (Für Lichtquellen mit Lichtabschwächung sind weitere
 *  Parameter zu speichern.)
 *
 *  Die Vereinbarungen in dieser Klasse müssen auf die
 *  im Shaderprogramm vereinbarten Variablen abgestimmt
 *  sein.
 */
class ShaderLight {
  public:
    void installProperties(LightProperties &properties);
    void readAddresses(GLuint shaderProgram, const GLchar *sourceName);

  private:
    GLint ambientColor;
    GLint diffuseColor;
    GLint specularColor;
    GLint position; 
    GLint isFixed;
    GLint isOn;

    GLint readAddress(GLuint shaderProgram,
                      const GLchar *sourceName,
                      const GLchar *varName);

};
#endif
