#include "ApplicationModel.hpp"
#include "OpenGLErrorHandler.hpp"

/*
 *  Der Konstruktur reserviert Speicher für die Speicherung der
 *  Geometriedaten des Szeneobjekts. Er erzeugt eine Textur,
 *  berechnet die Geometriedaten und legt Materialfarben und
 *  Lichtfarben fest. 
 */
ApplicationModel::ApplicationModel(Glib::RefPtr<Gtk::Application> a) : 
    app(a),
    c_nx(15),
    c_ny(201),
    //texture(Texture::createLuminanceTexture(256, 256, (GLubyte) 250, (GLubyte) 180)),
       //  Luminanztexturen sind eine Spezialität von OpenGL ES
    texture(Texture::createRGBTexture(256, 256, (GLubyte) 250, (GLubyte) 180)),
    ambientSceneLight { 0.3f, 0.3f, 0.3f, 1.0f },
    counter(0),
    phi(0.0f),
    parameterUpdated(true)
{
    sizeIndices = 6*(c_nx - 1)*(c_ny - 1);
    vertices = new GLfloat[3*c_nx*c_ny];
    vertexNormals = new GLfloat[3*c_nx*c_ny];

    materialFront = 
         {{ 0.6f, 0.0f, 0.0f, 1.0f },      // ambient
          { 0.0f, 0.6f, 0.7f, 1.0f },      // diffuse
          { 0.0f, 0.9f, 0.0f, 1.0f },      // specular
          { 0.3f, 0.3f, 0.0f, 1.0f },      // emissive
           70.0f
         };

    materialBack =  
         {{ 0.098f, 0.2f, 0.376f, 1.0f },  // ambient
          { 0.298f, 0.7f, 0.2f,   1.0f },  // diffuse
          { 0.0f,   0.8f, 0.6f,   1.0f },  // specular
          { 0.0f,   0.0f, 0.0f,   1.0f },  // emissive
           50.0f
         };

    lights[0] =
         {{  10.0f,  6.0f,  6.0f, 1.0f },  // position
          { 0.7f, 0.7f, 0.7f, 1.0f },      // ambient
          { 0.3f, 0.7f, 0.8f, 1.0f },      // diffuse
          { 1.0f, 0.0f, 1.0f, 1.0f },      // specular
          true,
          true
         };
    lights[1] =
         { { -10.0f, -6.0f, -6.0f, 1.0f }, // position
           { 0.5f, 0.5f, 0.5f, 1.0f },     // ambient
           { 0.1f, 0.6f, 0.2f, 1.0f },     // diffuse
           { 0.0f, 0.3f, 0.7f, 1.0f },     // specular
           true,
           true
         };

    //computeGeometryData(); 
}

/*
 *  Der Destruktur gibt den Speicher frei, der für die
 *  Speicherung der Geometriedaten des Szeneobjekts
 *  reserviert wurde.
 *  Die Textur wird hier nicht gelöscht, da ihr Destruktor
 *  nach den Regeln von C++ aufgerufen wird, wenn eine
 *  Instanz von  ApplicationModel freigegeben wird.
 */
ApplicationModel::~ApplicationModel() {
    delete [] vertices;
    delete [] vertexNormals;
}

void ApplicationModel::advanceAnimationParameter() {
    int newCounter = counter;

    newCounter += 1;
    if (newCounter == 360) {
        newCounter = 0;
    }
    counter = newCounter;
    phi = (float)(newCounter);
    parameterUpdated = true;
}

float ApplicationModel::getRotationAngle() {
    return phi;
}


/*
 *  Die Methode berechnet die Geometriedaten einer Tube mit
 *  Radius  radius  um die Raumkurve, die durch die in der
 *  Klasse bereitgestellten Funktionen  fn1, fn1d1, fn1d2
 *  definiert wird. 
 *  Aufrufparameter:
 *  nx       Anzahl der auf einem Kreis um einen Punkt
             der Raumkurve berechneten Stützstellen.
 *           Diese Zahl sollte ungerade sein, damit die
 *           Texturierung ein Schachbrettmuster liefert.
 *  ny       Anzahl der Stützstellen, die für die
             Raumkurve berechnet werden.
 *  y0       Anfangswert des Parameters der raumkurve.
 *  yStop    Endwert des Parameters der Raumkurve.
 *  radius   Der Radius der Tube um die Raumkurve. Der
 *           sollte im Verhältnis zum Durchmesser der
 *           Schraubenlinie nicht zu groß gewählt werden.
 */

void ApplicationModel::computeVerticesAndNormals
                 (int nx, int ny,
                  float y0, float yStop,
                  float radius)  {
    float stepPy = (yStop - y0)/(ny - 1);
    float deltaT = 2.0f*M_PI/(nx - 1);
    
    SpacialPoint fValue, d1Value, d2Value;
    SpacialPoint tangente, binormale, normale;
    SpacialPoint xx, pt;
    
    GLfloat *vertexPtr = vertices;
    GLfloat *vertexNormalsPtr = vertexNormals;
    
    for (int i = 0; i < ny; i++) {
        float t = y0 + i*stepPy;
        fn1(counter, t, &fValue);
        fn1d1(counter, t, &d1Value);
        fn1d2(counter, t, &d2Value);
        tangente = d1Value / d1Value.euclideanNorm();
        binormale = d1Value.cross(d2Value);
        if (binormale.euclideanNorm() < 1.0e-8) {
            binormale = SpacialPoint(-(tangente.getY() + tangente.getZ()),
                                     tangente.getX(),
                                     tangente.getX());
        }
        binormale = binormale / binormale.euclideanNorm();
        normale = binormale.cross(tangente);
        GLfloat *firstIndex = vertexPtr;
        GLfloat *firstNormalIdx = vertexNormalsPtr;
        for (int j = 0; j < nx  - 1; j++) {
            float phi = j * deltaT;
            xx = normale * sin(phi) + binormale*cos(phi);
            pt = fValue + xx*radius;
            *vertexPtr++ = pt.getX();
            *vertexPtr++ = pt.getY();
            *vertexPtr++ = pt.getZ();
            *vertexNormalsPtr++ = xx.getX();
            *vertexNormalsPtr++ = xx.getY();
            *vertexNormalsPtr++ = xx.getZ();
        }
        for (int j = 0; j < 3; j++) {
            *vertexPtr++ = *firstIndex++;
            *vertexNormalsPtr++ = *firstNormalIdx++;
        }
    }
}


/*
 *  Die Methode berechnet die Geometriedaten einer Tube mit
 *  Radius  radius  um die Raumkurve, die durch die in der
 *  Klasse bereitgestellten Funktionen  fn1, fn1d1, fn1d2
 *  definiert wird. 
 *  Aufrufparameter:
 *  nx       Anzahl der auf einem Kreis um einen Punkt
             der Raumkurve berechneten Stützstellen.
 *           Diese Zahl sollte ungerade sein, damit die
 *           Texturierung ein Schachbrettmuster liefert.
 *  ny       Anzahl der Stützstellen, die für die
             Raumkurve berechnet werden.
 *  y0       Anfangswert des Parameters der raumkurve.
 *  yStop    Endwert des Parameters der Raumkurve.
 *  radius   Der Radius der Tube um die Raumkurve. Der
 *           sollte im Verhältnis zum Durchmesser der
 *           Schraubenlinie nicht zu groß gewählt werden.
 */
void ApplicationModel::createInvariantGeometryData
                 (int nx, int ny,
                  GLushort *indices, GLfloat *textureCoordinates 
                 ) {
    GLfloat *texCoordPtr = textureCoordinates;
    float deltaX = 0.5f,
          deltaY = 0.5f;
       
    for (int i = 0; i < ny; i++) {
        float tt = i*deltaY;
        for (int j = 0; j < nx  - 1; j++) {
            float tu = j*deltaX;
            *texCoordPtr++ = tt;
            *texCoordPtr++ = tu;
        }
        *texCoordPtr++ = tt;
        *texCoordPtr++ = (nx - 1)*deltaX;
    }
    //  Berechnung der Mesh-Indices
    GLushort *indexPtr = indices; 
    for (int i = 0; i < nx - 1; i++) {
        for (int j = 0; j < ny - 1; j++) {
            unsigned int k = (unsigned int)(j*nx + i);
            *indexPtr++ = k;
            *indexPtr++ = k + nx;
            *indexPtr++ = k + 1;
            *indexPtr++ = k + 1;
            *indexPtr++ = k + nx;
            *indexPtr++ = k + nx + 1;
        }
    }
}


/*
 *  Die Methode berechnet die Indices des Gitters und die
 *  Texturkoordinaten und überträgt die Werte in die GPU. 
 *  Für das hier verwendete Szeneobjekt müssen die Gitterindices
 *  und die Texturkoordinaten nur ein enziges Mal berechnet werden.
 */
void ApplicationModel::computePermanentGeometryData
              (GLuint tcbuffer, GLuint elementbuffer) {
    GLushort     *indices;
    GLfloat      *textureCoordinates;

    //  Die Felder, die Werte aufnehmen, die nur ein einziges
    //  Mal berechnet und dann in die GPU übertragen werden,
    //  werden lokal in dieser Methode bereitgestellt und nach
    //  Gebrauch auch wieder freigegeben.
    indices = new GLushort [sizeIndices];
    textureCoordinates = new GLfloat[2*c_nx*c_ny];

    createInvariantGeometryData(c_nx, c_ny,
                                indices, textureCoordinates);

    glBindBuffer(GL_ARRAY_BUFFER, tcbuffer);
      OpenGLErrorHandler::checkAndReportErrors("glBindBuffer4");
    glBufferData(GL_ARRAY_BUFFER,
                 2*c_nx*c_ny * sizeof(GLfloat),
                 &(textureCoordinates[0]),
                 GL_STATIC_DRAW);
      OpenGLErrorHandler::checkAndReportErrors("glBufferData");  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
      OpenGLErrorHandler::checkAndReportErrors("glBindBuffer");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (sizeIndices) * sizeof(GLushort),
                 &(indices[0]),
                 GL_STATIC_DRAW); 
      
      OpenGLErrorHandler::checkAndReportErrors("glBufferData");

    delete [] indices;
    delete [] textureCoordinates;
}



/*
 *  Die Methode berechnet die Koordinaten der Vertices und die
 *  Vertexnormalen. Die berechneten Werte werden in die GPU
 *  übertragen. 
 *  Für das hier verwendete Szeneobjekt müssen die Vertices
 *  und die Vertexnormalen für jedes Szenebild neu berechnet
 *  werden, da die Animation die Geometrie des Szeneobjekts
 *  verändert.
 */
void ApplicationModel::computeVariableGeometryData
           (GLuint vertexbuffer, GLuint normalenbuffer) {
    if (parameterUpdated) {
        computeVerticesAndNormals(c_nx, c_ny, -3.5*M_PI, 3.5*M_PI, rt);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
          OpenGLErrorHandler::checkAndReportErrors("glBindBuffer");
        glBufferSubData(GL_ARRAY_BUFFER,
                 0,
                 3*c_nx*c_ny * sizeof(GLfloat),
                 &(vertices[0]));
          OpenGLErrorHandler::checkAndReportErrors("glBufferData");

        glBindBuffer(GL_ARRAY_BUFFER, normalenbuffer);
          OpenGLErrorHandler::checkAndReportErrors("glBindBuffer");
        glBufferSubData(GL_ARRAY_BUFFER,
                 0,
                 3*c_nx*c_ny * sizeof(GLfloat),
                 &(vertexNormals[0]));
          OpenGLErrorHandler::checkAndReportErrors("glBufferData");
        parameterUpdated = false;
        //std::cout << "geometry data updated" << std::endl; 
    } else {
        //std::cout << "no update" << std::endl;
    }
}

/*
 *  Funktionen für eine animierte Schraubenlinie.
 *  Die Ganghöhe der Schraubenlinie wird periodisch
 *  verändert. Es entsteht der Eindruck einer Feder,
 *  die abwechselnd einer Zug- und einer Druckkraft
 *  ausgesetzt ist.
 */
void ApplicationModel::fn1 (int cnt, float t, SpacialPoint *ft) {
    float arg = (float)(counter*3)/180.0*M_PI;
    *ft = SpacialPoint(radius*cos(t),
                       radius*sin(t),
                       (1 + sin(arg)/d)*ganghoehe*t
                      );
}

void ApplicationModel::fn1d1 (int cnt, float t, SpacialPoint *ft) {
    float arg = (float)(counter*3)/180.0*M_PI;
    *ft = SpacialPoint(-radius*sin(t),
                        radius*cos(t),
                       (1 + sin(arg)/d)*ganghoehe
                      );
}

void ApplicationModel::fn1d2 (int cnt, float t, SpacialPoint *ft) {
    *ft = SpacialPoint(-radius*cos(t),
                       -radius*sin(t),
                       0.0f
                      );
}
