#ifndef _APP_MODEL_HPP_
#define _APP_MODEL_HPP_

#include <gtkmm.h>       //   for Glib::RefPtr
#include "CustomizedGL.hpp"
#include "SpacialPoint.hpp"
#include "LightProperties.hpp"
#include "MaterialProperties.hpp"
#include "Texture.hpp"

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
        ApplicationModel(Glib::RefPtr<Gtk::Application> a);
        ~ApplicationModel();

    private:
        float const ganghoehe = 2.0f/3.0f;
        float const rt =  1.0f;
        float const radius = 4.0f;
        float const d = 4.0f;
 
        Glib::RefPtr<Gtk::Application> app;
        //  Bestandteile des Fensters:
        GtkWidget    *mainWindow;
        GtkWidget    *drawingArea;

    public:

        bool         hasShader;
        //  Geometriedaten:
        int           c_nx, c_ny;
        int           sizeIndices;
        Texture       texture;

        MaterialProperties materialFront, materialBack;
        LightProperties lights[2];
        GLfloat         ambientSceneLight[4];

        void advanceAnimationParameter();
        float getRotationAngle();

        void computePermanentGeometryData(GLuint tcbuffer, GLuint elementbuffer);
        void computeVariableGeometryData(GLuint vertexbuffer, GLuint normalenbuffer);

    private:
        // Parameter der Animation:
        int           counter;
        float         phi;
        bool          parameterUpdated;

        GLfloat      *vertices;
        GLfloat      *vertexNormals;


        void computeVerticesAndNormals
                 (int nx, int ny,
                  float t0, float tStop,
                  float radius
                 );

        void createInvariantGeometryData
                 (int nx, int ny,
                  GLushort *indices, GLfloat *textureCoordinates 
                 );

        void fn1   (int cnt, float t, SpacialPoint *ft);
        void fn1d1 (int cnt, float t, SpacialPoint *ft);
        void fn1d2 (int cnt, float t, SpacialPoint *ft);
};

#endif
