#ifndef _SHADER_VARIABLES_H_
#define _SHADER_VARIABLES_H_


#include "CustomizedGL.hpp"
#include "ShaderLight.hpp"
#include "ShaderMaterial.hpp"


#define C_NUM_LIGHTS 2
//  Es werden in diesem Beispiel nur zwei Lichter benötigt.


/*
 *  Die Struktur ShaderVariables_T  enthält die Adressen
 *  aller Variablen des Shaders.
 *  Diese Struktur ist für jeden Shader individuell zu
 *  definieren. 
 *  Hier werden die Adressen aller Variablen des Shaders
 *  gespeichert.  Die Adressen werden
 *  durch die Funtionen  glGetAttribLocation  und
 *  glGetUniformLocation   erfragt. Die Versorgung mit
 *  Werten geschieht durch die Funktionen
 *     glUniform, glUniformMatrix, glVertexAttribPointer
 *  und   glTexImage2D.
 *  
 */
typedef struct {
   GLint  mvp;                 // Adresse der Projektionsmatrix
   GLint  modelView;           // Adresse der Modell-View-Matrix
   GLint  invModelView;        // Adresse der invertierten Modell-View-Matrix
   GLint  ambientSceneLight;   // Adresse der Parameter für das Umgebungslicht
   GLint  enableLighting;      // Adresse einer Variablen, die anzeigt, ob Licht verwendet werden soll
   GLint  texture;             // Adresse der Bitmap der Textur
   GLint  useTexture;          // Adresse einer Variablen, die anzeigt, ob die Textur verwendet werden soll
   ShaderMaterial material;          //  Adressen der Materialparameter
   ShaderLight light[C_NUM_LIGHTS];  //  Adressen der Lichtparameter
}  ShaderVariables_T;

#endif
