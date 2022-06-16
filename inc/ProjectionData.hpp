#ifndef _PROJECTION_DATA_HPP_
#define _PROJECTION_DATA_HPP_

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

/*
 *  Die Klasse speichert die für die Abbildungsformeln
 *  benötigten Matrizen.
 *  Die Methoden der Klasse bilden aus den gepeicherten
 *  Matrizen durch Komposition weitere Matrizen, die
 *  dem Shaderprogramm zur Verfügung gestellt werden müssen.
 */
class ProjectionData {
    public:
      
     glm::mat4 projection;
     glm::mat4 view;  

     glm::mat4 translationMatrix; 
     glm::mat4 rotationMatrix; 
     glm::mat4 scaleMatrix;
     
     glm::mat4 getMVPMatrix();
     glm::mat4 getModelViewMatrix();
     glm::mat4 getModelMatrix();
};
#endif
