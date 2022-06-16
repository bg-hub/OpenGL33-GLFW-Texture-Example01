#include "ProjectionData.hpp"


glm::mat4 ProjectionData::getMVPMatrix() {
    return projection*view*getModelMatrix();
}

glm::mat4 ProjectionData::getModelViewMatrix() {
    return view*getModelMatrix();
}

glm::mat4 ProjectionData::getModelMatrix() {
    return translationMatrix*rotationMatrix*scaleMatrix;
}
