#include "ShaderInterface.hpp"
#include "OpenGLErrorHandler.hpp"
#include <stddef.h>   //  für  NULL

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

void ShaderInterface::setVerbose(bool value) {
    shaderProgram.setVerbose(value);
}

/*
 *  Die beiden Shader werden compiliert und das Shaderprogramm wird
 *  hergestellt. Die Adressen aller Variablen des Shaderprogramms
 *  werden erfragt und für die spätere Verwendung gespeichert.
 *  VBOs und VAO für das Szeneobjekt werden hier erzeugt. Ebenso
 *  wird die Textur hier erzeugt.
 */
void ShaderInterface::initShaderInterface(ApplicationModel &model) {
    shaderProgram.createShaderProgram();
    queryAdresses();

    //  Es folgt die Erzeugung der langlebigen Objekte

    glGenBuffers(1, &tcbuffer);
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &normalenbuffer);
    glGenBuffers(1, &elementbuffer);
    
    model.computePermanentGeometryData(tcbuffer, elementbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      OpenGLErrorHandler::checkAndReportErrors("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER,
                 3*model.c_nx*model.c_ny * sizeof(GLfloat),
                 NULL,
                 GL_DYNAMIC_DRAW);
      OpenGLErrorHandler::checkAndReportErrors("glBufferData");

    glBindBuffer(GL_ARRAY_BUFFER, normalenbuffer);
      OpenGLErrorHandler::checkAndReportErrors("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER,
                 3*model.c_nx*model.c_ny * sizeof(GLfloat),
                 NULL,
                 GL_DYNAMIC_DRAW);
      OpenGLErrorHandler::checkAndReportErrors("glBufferData");

    //  Textur erzeugen und Texturparameter festlegen:
    glGenTextures(1, &textureId[0]);
      OpenGLErrorHandler::checkAndReportErrors("genTextures");
    
    glActiveTexture(GL_TEXTURE0);
      OpenGLErrorHandler::checkAndReportErrors("glActivTexture");
    glBindTexture(GL_TEXTURE_2D, textureId[0]);
      OpenGLErrorHandler::checkAndReportErrors("glBindTexture");
    
    model.texture.installTexture();
    

    //  Herstellung des VAO, das die Attribute des Szeneobjekts
    //  zusammenfasst.
    glGenVertexArrays(1, &VertexArrayID);
      OpenGLErrorHandler::checkAndReportErrors("glGenVertexArrays");
    glBindVertexArray(VertexArrayID);
      OpenGLErrorHandler::checkAndReportErrors("glBindVertexArray");

    glEnableVertexAttribArray(0);  //  Texturkoordinaten
    glBindBuffer(GL_ARRAY_BUFFER, tcbuffer);
    glVertexAttribPointer(0, 2,
                          GL_FLOAT, GL_FALSE, 0, (void*) 0);
      OpenGLErrorHandler::checkAndReportErrors("glVertexAttribPointer texture Coordinate");

    glEnableVertexAttribArray(1);  //  Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(1, 3,
                          GL_FLOAT, GL_FALSE, 0, (void*)0);
      OpenGLErrorHandler::checkAndReportErrors("glVertexAttribPointer 1");

    glEnableVertexAttribArray(2);  //  Vertexnormalen
    glBindBuffer(GL_ARRAY_BUFFER, normalenbuffer);
    glVertexAttribPointer(2, 3,
                          GL_FLOAT, GL_FALSE, 0, (void*)0);
      OpenGLErrorHandler::checkAndReportErrors("glVertexAttribPointer 2");

         //  Meshindices ins VAO eintragen
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
      OpenGLErrorHandler::checkAndReportErrors("glBindBuffer (vor glDrawElements)");
    
    glBindVertexArray(0);   //  Herstellung des VAO abgeschlossen
    model.hasShader = true;
}

/*
 *  Die Methode gibt beide Shader und das Shaderprogramm frei.
 *  Diese Methode wird aufgerufen, wenn das Ereignis  unrealize
 *  des Widgets OpenGLSceneView behandelt wird.
 */
void ShaderInterface::releaseShaderProgram() {
    //  VBOs löschen
    glDeleteBuffers(1, &tcbuffer);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &normalenbuffer);
    glDeleteBuffers(1, &elementbuffer);
    //  Textur löschen
    glDeleteTextures(1, textureId);
    //  VAO löschen
    glDeleteVertexArrays(1, &VertexArrayID);
}

/*
 *  Die Methode erfragt die Adressen der Variablen des Shaderprogramms
 *  und speichert sie für die spätere Verwendung.
 */
void  ShaderInterface::queryAdresses() {
    GLuint programID = shaderProgram.getProgramID();
    //  Matrizen der Projektionsformeln:
    shaderVariables.mvp = glGetUniformLocation(programID, "mvp");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    shaderVariables.modelView = glGetUniformLocation(programID, "modelView");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    shaderVariables.invModelView = glGetUniformLocation(programID, "invModelView");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
 
    //  Geometriedaten des Szeneobjekts:
    shaderVariables.useTexture = glGetUniformLocation(programID, "useTexture");
    OpenGLErrorHandler::checkAndReportErrors("glGetAttribLocation");
    shaderVariables.texture = glGetUniformLocation(programID, "texture");
    OpenGLErrorHandler::checkAndReportErrors("glGetAttribLocation");
    
    //  Optionen für die Verwendung von Lichtquellen:
    shaderVariables.ambientSceneLight = glGetUniformLocation(programID, "ambientSceneLight");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    shaderVariables.enableLighting = glGetUniformLocation(programID, "enableLighting");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");

    // Parameter der Lichtquellen:
    shaderVariables.light[0].readAddresses(programID, "light0");
    shaderVariables.light[1].readAddresses(programID, "light1");

    //  Materialeigenschaften des Szeneobjekts
    shaderVariables.material.readAddresses(programID);
}


/*
 *  Die Methode versorgt die GPU mit den Daten, die für die
 *  Bilderzeugung gebraucht werden.
 */
void ShaderInterface::renderImage(ApplicationModel &model) {

    model.computeVariableGeometryData
           (vertexbuffer, normalenbuffer);

    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix(glm::scale<GLfloat>(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f)));
    glm::vec3 rotationAxis1(glm::vec3(1.0f, 0.5f, 0.4f));
    glm::mat4 rotationMatrix1(glm::rotate(identityMatrix, glm::radians(285.0f), rotationAxis1));
    glm::vec3 rotationAxis2(glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotationMatrix2(glm::rotate(identityMatrix, glm::radians(model.getRotationAngle()), rotationAxis2));
    glm::mat4 translationMatrix(glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f)));
    
    pd.scaleMatrix = scaleMatrix;
    pd.translationMatrix = translationMatrix;
    pd.rotationMatrix = rotationMatrix1*rotationMatrix2;

    if (model.orthoProjection) {  //  Parallelprojektion
        pd.view =
            glm::lookAt<GLfloat>(
                glm::vec3(0.0f, 0.0f, 2.0f),    //  Position der Kamera
                glm::vec3(0.0f, 0.0f, 0.0f),    //  target
                glm::vec3(0.0f, 1.0f, 0.0f));   //  up-vector
    
        //  Bestimmung des Szenequaders der Parallelprojektion
        pd.projection =
           glm::ortho<GLfloat>(
                -6.0f,                //  left
                 6.0f,                //  right
                -6.0f,                //  bottom
                 6.0f,                //  top
                -7.0f,                //  zNear 
                 7.0f);               //  zFar
    } else {  //  Zentralprojektion
        pd.view =
            glm::lookAt<GLfloat>(
                glm::vec3(0.0f, 0.0f, 12.0f),    //  Position der Kamera
                glm::vec3(0.0f, 0.0f,  0.0f),    //  target
                glm::vec3(0.0f, 1.0f,  0.0f));   //  up-vector

        pd.projection =
           glm::perspective<GLfloat>(
                glm::radians(56.0f),  //  fov
                1.0f,                 //  aspect
                0.5f,                 //  near
                19.0f);               //  far  
    }

    glUseProgram(shaderProgram.getProgramID());
      OpenGLErrorHandler::checkAndReportErrors("glUseProgram");

    //  Das erste und das zweite Argument dieses Aufrufs von
    //  glViewport  werden so berechnet, dass der viewport
    //  mittig platziert wird.
    glViewport((GLint) model.screen_width > model.image_width
                           ? (model.screen_width - model.image_width)/2
                           : 0,
               (GLint) model.screen_height > model.image_width
                           ? (model.screen_height - model.image_width)/2
                           : 0, 
               (GLsizei) model.image_width,
               (GLsizei) model.image_width);
      OpenGLErrorHandler::checkAndReportErrors("glViewport");
    glEnable(GL_DEPTH_TEST);
      OpenGLErrorHandler::checkAndReportErrors("glEnable GL_DEPTH_TEST");
    glDepthFunc(GL_LESS);
      OpenGLErrorHandler::checkAndReportErrors("glDepthFunc");
    glClearDepthf(1.0f);
      OpenGLErrorHandler::checkAndReportErrors("glClearDepth");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      OpenGLErrorHandler::checkAndReportErrors("glClear");
    
    //  Abbildungsmatrizen bereitstellen
    glm::mat4 mvp = pd.getMVPMatrix();
    glUniformMatrix4fv(shaderVariables.mvp, 1, false, glm::value_ptr(mvp));
      OpenGLErrorHandler::checkAndReportErrors("set mvp matrix");

    glm::mat4 modelView = pd.getModelViewMatrix();
    glUniformMatrix4fv(shaderVariables.modelView, 1, false, glm::value_ptr(modelView));
       OpenGLErrorHandler::checkAndReportErrors("set modelView matrix");
     
    glm::mat3 glNormal;
    glNormal = glm::transpose(glm::inverse(glm::mat3(modelView)));
    glUniformMatrix3fv(shaderVariables.invModelView, 1, false, glm::value_ptr(glNormal));
       OpenGLErrorHandler::checkAndReportErrors("set invmodelView matrix");

    //  Konfigurieren der Lichtquellen    
    glUniform4fv(shaderVariables.ambientSceneLight, 1, model.ambientSceneLight);
      OpenGLErrorHandler::checkAndReportErrors("set ambient light");

    glUniform1i(shaderVariables.enableLighting, 1);
      OpenGLErrorHandler::checkAndReportErrors("switch light 0 on");

    shaderVariables.light[0].installProperties(model.lights[0]);
    shaderVariables.light[1].installProperties(model.lights[1]);

    //  Textur bereitstellen
    glUniform1i(shaderVariables.useTexture,
                model.showTexture  ? 1 : 0);  
      OpenGLErrorHandler::checkAndReportErrors("glUniform1i(useTexture, )");
    glActiveTexture(GL_TEXTURE0);
      OpenGLErrorHandler::checkAndReportErrors("glActivTexture");
    glBindTexture(GL_TEXTURE_2D, textureId[0]);
      OpenGLErrorHandler::checkAndReportErrors("glBindTexture");
    glUniform1i(shaderVariables.texture, 0);  //  Versorgung des Samplers
      OpenGLErrorHandler::checkAndReportErrors("glUniform1i(sampler )");

    //  Geometriedaten des Szeneobjekts bereitstellen
    glBindVertexArray(VertexArrayID);
      OpenGLErrorHandler::checkAndReportErrors("glBindVertexArray");

    //  Vorderseite und Rückseite des Szeneobjekts werden
    //  nun nacheinander mit verschiedenen Materialfarben
    //  gezeichnet. Die Anordnung der Lichtquellen und die
     //  Lichtfarben sind für Vorderseite und Rückseite gleich.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);   //  Rückseite verbergen

    shaderVariables.material.installProperties(model.materialFront);

    glDrawElements(GL_TRIANGLES, model.sizeIndices,
                   GL_UNSIGNED_SHORT, (void*) 0);    /// Vorderseite zeichnen
      OpenGLErrorHandler::checkAndReportErrors("glDrawElements");

    glCullFace(GL_FRONT);  // Vorderseite verbergen

    shaderVariables.material.installProperties(model.materialBack);

    glDrawElements(GL_TRIANGLES, model.sizeIndices,
                   GL_UNSIGNED_SHORT, (void*) 0);    /// Rueckseite zeichnen
      OpenGLErrorHandler::checkAndReportErrors("glDrawElements");

    glDisable(GL_CULL_FACE);

    glBindVertexArray(0);

    glFlush();
    glFinish();
}
