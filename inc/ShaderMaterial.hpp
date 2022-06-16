#ifndef _SHADER_MATERIAL_H_
#define _SHADER_MATERIAL_H_

#include "CustomizedGL.hpp"
#include "MaterialProperties.hpp"

/*
 *  Eine Instanz speichert die Adressen der Konstanten
 *  des Shaderprogramms, die die Eigenschaften eines
 *  Materials bestimmen.
 *  Eine Instanzmethode leistet die Übertragung der Werte
 *  an die GPU.
 *  Eine gleichartige Struktur wird im Vertex Shader für
 *  für die Darstellung der Eigenschaften eines Materials
 *  verwendet.
 */
class ShaderMaterial {
  public:
    void installProperties(MaterialProperties &properties);
    void readAddresses(GLuint shaderProgram);

  private:
    GLint ambientColor;
    GLint diffuseColor;
    GLint specularColor;
    GLint emissiveColor;
    GLint specularExponent;
};

#endif
