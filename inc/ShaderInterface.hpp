#ifndef _SHADER_INTERFACE_H_
#define _SHADER_INTERFACE_H_


#include "CustomizedGL.hpp"
#include "ApplicationModel.hpp"
#include "DisplayRectangle.hpp"
#include "ProjectionData.hpp"
#include "ShaderProgram.hpp"
#include "ShaderVariables.hpp"

/*
 *  Eine Instanz dieser Klasse realisiert die Kooperation
 *  mit dem in GLSL geschriebenen Shaderprogramm.
 *  Die Instanz speichert die Adressen der Variablen des
 *  Shaderprogramms und weist diesen Variablen Werte zu. 
 *
 *  Die Vereinbarungen in dieser Klasse müssen auf die
 *  im Shaderprogramm vereinbarten Variablen abgestimmt
 *  sein. Änderungen im Shaderprogramm (etwa die Hinzufügung
 *  weiterer Lichtquellen) müssen immer auch in dieser
 *  Klasse vorgenommen werden!
 */
class ShaderInterface {
     
    public:
     ShaderInterface();
     ~ShaderInterface();

     void initShaderInterface(ApplicationModel &model);
     void releaseShaderProgram();
     void setVerbose(bool value);
     void renderImage(ApplicationModel &model, DisplayRect &displayRect);

  private:
     ShaderProgram      shaderProgram;
     ShaderVariables_T  shaderVariables;
     ProjectionData     pd;

     //  VAO und VBOs für Geometriedaten
     GLuint VertexArrayID;     //  Vertex Array Object des Szeneobjekts
     GLuint vertexbuffer;      //  für die Vertices
     GLuint normalenbuffer;    //  für die Vertexnormalen
     GLuint tcbuffer;          //  für die Texturkoordinaten
     GLuint elementbuffer;     //  für die Indices des Gitters

     GLuint textureId[1];      //  für die Textur

     void queryAdresses();
     void createVBOs(ApplicationModel &model);
     void createVAO();
     void createTexture(ApplicationModel &model);
};

#endif
