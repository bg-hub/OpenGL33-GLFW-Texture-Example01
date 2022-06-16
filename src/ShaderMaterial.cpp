#include "ShaderMaterial.hpp"
#include "OpenGLErrorHandler.hpp"

/*
 *  Die Methode versorgt das Shaderprogramm mit Werten, die
 *  im Parameter  properties  bereitgestellt werden.
 *  Der Verwendung dieser Methode muss der einmalige Aufruf
 *  der Methode  readAddresses  vorausgehen.
 */
void ShaderMaterial::installProperties(MaterialProperties &properties) {
    glUniform4fv(ambientColor, 1, properties.ambientColor);
      OpenGLErrorHandler::checkAndReportErrors("set ambient color");
    glUniform4fv(diffuseColor, 1, properties.diffuseColor);
      OpenGLErrorHandler::checkAndReportErrors("set diffuse color");
    glUniform4fv(specularColor, 1, properties.specularColor);
      OpenGLErrorHandler::checkAndReportErrors("set specular color");
    glUniform4fv(emissiveColor, 1, properties.emissiveColor);
      OpenGLErrorHandler::checkAndReportErrors("set emissive color");
    glUniform1f(specularExponent, (GLfloat)properties.shininess);
      OpenGLErrorHandler::checkAndReportErrors("set shininess");
}

/*
 *  Die Methode ermittelt die Adressen der im Vertex Sshader
 *  vereinbarten Programmparameter.
 */
void ShaderMaterial::readAddresses(GLuint shaderProgram) {
    ambientColor = 
        glGetUniformLocation(shaderProgram, "material.ambientColor");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    diffuseColor = 
        glGetUniformLocation(shaderProgram, "material.diffuseColor");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    specularColor = 
        glGetUniformLocation(shaderProgram, "material.specularColor");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    emissiveColor = 
        glGetUniformLocation(shaderProgram, "material.emissiveColor");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    specularExponent = 
        glGetUniformLocation(shaderProgram, "material.specularExponent");
}
