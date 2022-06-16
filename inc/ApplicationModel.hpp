#ifndef _APP_MODEL_HPP_
#define _APP_MODEL_HPP_

#include "CustomizedGL.hpp"
#include "SpacialPoint.hpp"
#include "LightProperties.hpp"
#include "MaterialProperties.hpp"
#include "Texture.hpp"
#include "VectorTriple.hpp"

/*
 *  Eine Instanz der Klasse  ApplicationModel  speichert alle
 *  Daten, die für die Versorgung des Fensters der Anwendung
 *  und für die Realisierung der Programmfunktionen benötigt
 *  werden. 
 *  Die Methoden der Klasse leisten die rechnerische Herstellung
 *  der Geometriedaten des Szeneobjekts.
 */
class ApplicationModel {
    public:
        ApplicationModel();
        ~ApplicationModel();


        int          screen_width, screen_height, image_width;
        bool         hasShader;
        bool         showTexture;
        bool         orthoProjection;

        //  Geometriedaten:
        int           c_nx, c_ny;
        int           sizeIndices;
        Texture       texture;

        MaterialProperties materialFront, materialBack;
        LightProperties lights[2];
        GLfloat         ambientSceneLight[4];

        void advanceAnimationParameter();
        float getRotationAngle();
        void toggleTexture();
        void toggleProjection();

        void computePermanentGeometryData(GLuint tcbuffer, GLuint elementbuffer);
        void computeVariableGeometryData(GLuint vertexbuffer, GLuint normalenbuffer);

    private:
        int const    hoehe = 2.3;
        int const    blaetter = 5;
 
    public:
        // Parameter der Animation:
        int          counter;
        float        phi;
        int          delay;
        int          delayCounter;
        bool         parameterUpdated;

    private:
        GLfloat      *vertices;
        GLfloat      *vertexNormals;

        void computeVerticesAndNormals
                 (int nx, int ny,
                  float x0, float xStop,
                  float y0, float yStop);
        void createInvariantGeometryData
                 (int nx, int ny,
                  GLushort *indices, GLfloat *textureCoordinates 
                 );
        void fnp (float r, float t, VectorTriple *ft);
};

#endif
