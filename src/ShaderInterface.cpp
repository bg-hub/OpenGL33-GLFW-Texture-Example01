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

//#include <iostream>

ShaderInterface:: ShaderInterface() {
    shaderProgram.verbose = true;
}
/*
 *  Die Methode gibt beide Shader und das Shaderprogramm frei.
 *  Diese Methode wird aufgerufen, wenn das Ereignis  unrealize
 *  des Widgets OpenGLSceneView behandelt wird.
 */
 ShaderInterface:: ~ShaderInterface() {
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

void ShaderInterface::setVerbose(bool value) {
    shaderProgram.verbose = value;
}

/*
 *  Die beiden Shader werden compiliert und das Shaderprogramm wird
 *  hergestellt. Die Adressen aller Variablen des Shaderprogramms
 *  werden erfragt und für die spätere Verwendung gespeichert.
 *  VBOs und VAO für das Szeneobjekt werden hier erzeugt. Ebenso
 *  werden die unveränderlichen Geometriedaten des Szeneobjekts
 *  und die Textur hier erzeugt.
 */
void ShaderInterface::initShaderInterface(ApplicationModel &model) {
    shaderProgram.init_shader_interface();
    queryAdresses();
    createVBOs(model);
    createVAO();
    createTexture(model);
    model.computePermanentGeometryData(tcbuffer, elementbuffer);
    model.hasShader = true;
}


/*
 *
 */
void ShaderInterface::createVBOs(ApplicationModel &model) {
    //  Es folgt die Erzeugung der langlebigen Objekte
    //  Zunächst werden die VBOs hergestellt und mit Daten
    //  versorgt, sodann wird das VAO hergestellt, das
    //  später verwendet wird, um die für die Bilderzeugung
    //  erforderlichen Daten auf einfache Weise bereitzustellen.

    glGenBuffers(1, &tcbuffer);
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &normalenbuffer);
    glGenBuffers(1, &elementbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, tcbuffer);
      OpenGLErrorHandler::checkAndReportErrors("01");
    glBufferData(GL_ARRAY_BUFFER,
                 2*model.c_nx*model.c_ny * sizeof(GLfloat),
                 NULL,
                 GL_STATIC_DRAW);
      OpenGLErrorHandler::checkAndReportErrors("02");
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      OpenGLErrorHandler::checkAndReportErrors("03");
    glBufferData(GL_ARRAY_BUFFER,
                 3*model.c_nx*model.c_ny * sizeof(GLfloat),
                 NULL,
                 GL_DYNAMIC_DRAW);
      OpenGLErrorHandler::checkAndReportErrors("04");

    glBindBuffer(GL_ARRAY_BUFFER, normalenbuffer);
      OpenGLErrorHandler::checkAndReportErrors("05");
    glBufferData(GL_ARRAY_BUFFER,
                 3*model.c_nx*model.c_ny * sizeof(GLfloat),
                 NULL,
                 GL_DYNAMIC_DRAW);
      OpenGLErrorHandler::checkAndReportErrors("06");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
      OpenGLErrorHandler::checkAndReportErrors("07");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (model.sizeIndices) * sizeof(GLushort),
                 NULL,
                 GL_STATIC_DRAW);
      OpenGLErrorHandler::checkAndReportErrors("08");
}

/*
 *  Herstellung eines Vertex Array Objects, das die Geometriedaten
 *  des Szeneobjekts bereitstellt. Das Vertex Array Object wird später
 *  verwendet, um alle hier aufgeführtes VBOs mit einer einzigen
 *  Bind-Anweisung bereitzustellen.
 *  Diese Funktion kann erst aufgerufen werden, nachdem die
 *  Vertex Buffer Objects mit einem Aufruf von  createVBOs
 *  bereitgestellt wurden.
 */
void ShaderInterface::createVAO() {
    //   Herstellung des VAO
    glGenVertexArrays(1, &VertexArrayID);
      OpenGLErrorHandler::checkAndReportErrors("glGenVertexArrays");
    glBindVertexArray(VertexArrayID);
      OpenGLErrorHandler::checkAndReportErrors("glBinfVertexArray");

    //  Registrierung der VBOs des Szeneobjekts im VAO.
    //  Die VBOs stellen die Geometriedaten des Szeneobjekts bereit
    glEnableVertexAttribArray(0);   //  Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE, 0, (void*)0);
      OpenGLErrorHandler::checkAndReportErrors("glVertexAttribPointer 1");

    glEnableVertexAttribArray(1);   //  Normalenvektoren
    glBindBuffer(GL_ARRAY_BUFFER, normalenbuffer);
    glVertexAttribPointer(1, 3,
                          GL_FLOAT, GL_FALSE, 0, (void*)0);
      OpenGLErrorHandler::checkAndReportErrors("glVertexAttribPointer 2");

    glEnableVertexAttribArray(2);   //  Texturkoordinaten
    glBindBuffer(GL_ARRAY_BUFFER, tcbuffer);
    glVertexAttribPointer(2, 2,
                          GL_FLOAT, GL_FALSE, 0, (void*) 0);
      OpenGLErrorHandler::checkAndReportErrors("glVertexAttribPointer texture Coordinate");

         //  Meshindices ins VAO eintragen
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
      OpenGLErrorHandler::checkAndReportErrors("glBindBuffer (vor glDrawElements)");

    glBindVertexArray(0);   //  Herstellung des VAO abgeschlossen
}

/*
 *  Pufferspeicher für die Textur erzeugen, Texturparameter
 *  festlegen und die Textur in die GPU übertragen
 */
void ShaderInterface::createTexture(ApplicationModel &model) {
    //  Textur erzeugen und Texturparameter festlegen:
    glGenTextures(1, &textureId[0]);
      OpenGLErrorHandler::checkAndReportErrors("genTextures");
    glActiveTexture(GL_TEXTURE0);
      OpenGLErrorHandler::checkAndReportErrors("glActivTexture");
    glBindTexture(GL_TEXTURE_2D, textureId[0]);
      OpenGLErrorHandler::checkAndReportErrors("glBindTexture");
    glUniform1i(shaderVariables.texture, 0);  //  Versorgung des Samplers
      OpenGLErrorHandler::checkAndReportErrors("glUniform1i(useTexture, )");
    model.texture.installTexture();
}

/*
 *  Die Methode erfragt die Adressen der Variablen des Shaderprogramms
 *  und speichert sie für die spätere Verwendung.
 */
void  ShaderInterface::queryAdresses() {
    //  Matrizen der Projektionsformeln:
    shaderVariables.mvp = glGetUniformLocation(shaderProgram.program, "mvp");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    shaderVariables.modelView = glGetUniformLocation(shaderProgram.program, "modelView");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    shaderVariables.invModelView = glGetUniformLocation(shaderProgram.program, "invModelView");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
 
    //  Texturvariablen:
    shaderVariables.useTexture = glGetUniformLocation(shaderProgram.program, "useTexture");
    OpenGLErrorHandler::checkAndReportErrors("glGetAttribLocation");
    shaderVariables.texture = glGetUniformLocation(shaderProgram.program, "texture");
    OpenGLErrorHandler::checkAndReportErrors("glGetAttribLocation");
    
    //  Optionen für die Verwendung von Lichtquellen:
    shaderVariables.ambientSceneLight = glGetUniformLocation(shaderProgram.program, "ambientSceneLight");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");
    shaderVariables.enableLighting = glGetUniformLocation(shaderProgram.program, "enableLighting");
    OpenGLErrorHandler::checkAndReportErrors("glGetUniformLocation");

    // Parameter der Lichtquellen:
    shaderVariables.light[0].readAddresses(shaderProgram.program, "light0");
    shaderVariables.light[1].readAddresses(shaderProgram.program, "light1");

    //  Materialeigenschaften des Szeneobjekts
    shaderVariables.material.readAddresses(shaderProgram.program);
}


/*
 *  Die Methode versorgt die GPU mit den Daten, die für die
 *  Bilderzeugung gebraucht werden.
 */
void ShaderInterface::renderImage(ApplicationModel &model, DisplayRect &displayRect) {

    //  Bereitstellung der veränderlichen Geometriedaten
    model.computeVariableGeometryData(vertexbuffer, normalenbuffer);


    //  Einrichtung der Szene
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

    pd.view =
            glm::lookAt<GLfloat>(
                glm::vec3(0.0f, 0.0f, 2.0f),    //  Position der Kamera
                glm::vec3(0.0f, 0.0f, 0.0f),    //  target
                glm::vec3(0.0f, 1.0f, 0.0f));   //  up-vector

    /* 
    //  Für eine Zentralprojektion wäre zu schreiben:
    pd.projection =
           glm::perspective<GLfloat>(
                glm::radians(45.0f),  //  fov
                1.0f,                 //  aspect
                0.5f,                 //  near
                10.0f);               //  far  
    */
         
    //  Bestimmung des Szenequaders der Parallelprojektion
    pd.projection =
           glm::ortho<GLfloat>(
                -11.0f,                //  left
                 11.0f,                //  right
                -11.0f,                //  bottom
                 11.0f,                //  top
                -11.0f,                //  zNear 
                 11.0f);               //  zFar

    glUseProgram(shaderProgram.program);
      OpenGLErrorHandler::checkAndReportErrors("glUseProgram");

    //  Vorbereitung der Bilderzeugung
    //  Die linke obere Ecke des viewport wird so festgelegt,
    //  dass die Szene im Fenster eingemittet erscheint.
    glViewport((GLint) (displayRect.screenWidth > displayRect.imageWidth
                            ? (displayRect.screenWidth - displayRect.imageWidth) / 2
                            : 0),
               (GLint) (displayRect.screenHeight > displayRect.imageWidth
                            ? (displayRect.screenHeight - displayRect.imageWidth) / 2
                            : 0),
               (GLsizei)displayRect.imageWidth,
               (GLsizei)displayRect.imageWidth);
      OpenGLErrorHandler::checkAndReportErrors("glViewport");

    //  Allgeimeine Einstellungen
    glEnable(GL_DEPTH_TEST);
      OpenGLErrorHandler::checkAndReportErrors("glEnable GL_DEPTH_TEST");
    glDepthFunc(GL_LESS);
      OpenGLErrorHandler::checkAndReportErrors("glDepthFunc");
    glClearDepthf(1.0f);
      OpenGLErrorHandler::checkAndReportErrors("glClearDepth");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      OpenGLErrorHandler::checkAndReportErrors("glClear");
    
    //  Übertragung der Abbildungsmatrizen
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

    glUniform1i(shaderVariables.useTexture, 1);  
      OpenGLErrorHandler::checkAndReportErrors("glUniform1i(useTexture, )");

    //  Geometriedaten des Szeneobjekts bereitstellen
    glBindVertexArray(VertexArrayID);

    //  Vorderseite und Rückseite des Szeneobjekts werden
    //  nun nacheinander mit verschiedenen Materialfarben
    //  gezeichnet. Die Anordnung der Lichtquellen und die
     //  Lichtfarben sind für Vorderseite und Rückseite gleich.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);   //Rückseite verbergen
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
