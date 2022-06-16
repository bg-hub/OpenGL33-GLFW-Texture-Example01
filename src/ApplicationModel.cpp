#include "ApplicationModel.hpp"
#include "OpenGLErrorHandler.hpp"

/*
 *  Der Konstruktor initialisiert eine Instanz mit den Parametern der
 *  Animation, den Materialfarben des Szeneobjekts und den
 *  Lichteigenschaften. Darüber hinaus wird Speicher für jene
 *  Geometriedaten reserviert, die für jedes Szenebild der
 *  Animation neu berechnet werden müssen.
 */
ApplicationModel::ApplicationModel() : 
    showTexture(false),
    orthoProjection(true),
    c_nx(12),
    c_ny(117),
    texture(Texture::createRGBTexture(256, 256, (GLubyte) 250, (GLubyte) 180)),
    ambientSceneLight { 0.3f, 0.3f, 0.3f, 1.0f },
    counter(0),
    phi(0.0f),
    delay(0),
    delayCounter(0),
    parameterUpdated(true)
{
    sizeIndices = 6*(c_nx - 1)*(c_ny - 1);

    //  Die Felder, die Werte aufnehmen, die für jedes
    //  Szenebild neu zu berechnen wind, werden als
    //  Instanzvariablen vereinbart, um das häufige
    //  Erzeugen und Freigeben von Speicherbereichen
    //  zu vermeiden.
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
}

ApplicationModel::~ApplicationModel() {
    delete [] vertices;
    delete [] vertexNormals;
    //  Nach dem Verlassen dieses Destruktors werden
    //  die Destruktoren der Instanzvariablen ausgeführt.
    //  Die einzige Instanzvariable, für die ein Destruktor
    //  tatsächlich ausgeführt wird, ist die Variable
    //  textur 
}


/*
 *  Ein Aufruf dieser Methode aktualisiert die Animations-
 *  parameter der Szene. Damit wird das Fortschreiten der
 *  Animation vorbereitet.
 *
 *  phi  ist der Rotationswinkel in Grad,  counter ist der
 *  Flächendeformationsparameter.
 */
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

/*
 *  Die Methode gibt den aktuellen Rotationswinkel des
 *  Szeneobjekts zurück.
 *
 *  Aufruf in  ShaderInterface>>renderImage
 */
float ApplicationModel::getRotationAngle() {
    return phi;
}

/*
 *  Die Methode schaltet den Anzeigemodus für die Textur um.
 *  Aufruf in  keyboardInputCallback  (in Datei  main.cpp)
 */
void ApplicationModel::toggleTexture() {
    showTexture = !showTexture;
}

/*
 *  Die Methode schaltet den Anzeigemodus für die
 *  Projektion um.
 *  Aufruf in  keyboardInputCallback  (in Datei  main.cpp)
 */
void ApplicationModel::toggleProjection() {
    orthoProjection = ! orthoProjection;
}

/*
 *  Die Methode berechnet die Indices des Gitters und die
 *  Texturkoordinaten und überträgt die Werte in die 
 *  vorbereiteten VBOs. 
 *  Für das hier verwendete Szeneobjekt müssen die Gitterindices
 *  und die Texturkoordinaten nur ein enziges Mal berechnet werden.
 *
 *  Parameter:
 *    tcbuffer
 *       ID des Vertex Buffer Objects, das die Texturkoordinaten
 *       aufnimmt.
 *    elementbuffer
 *       ID des Vertex Buffer Objects, das die Gitterindices
 *       aufnimmt, aus denen das Dreiecksnetz konstruiert wird.
 *
 *  Aufruf in  SgaderInterface>>initShaderInterface
 */
void ApplicationModel::computePermanentGeometryData
              (GLuint tcbuffer, GLuint elementbuffer) {
    GLushort     *indices;
    GLfloat      *textureCoordinates;

    //  Die Felder, die Werte aufnehmen, die nur ein einziges
    //  rechnet und dann in die GPU übertragen werden, werden
    //  lokal in dieser Methode bereitgestellt.
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
 *  Vertexnormalen. Die berechneten Werte werden in die 
 *  vorbereiteten VBOs übertragen. 
 *  Für das hier verwendete Szeneobjekt müssen die Vertices
 *  und die Vertexnormalen für jedes Szenebild neu berechnet
 *  werden, da die Animation die Geometrie des Szeneobjekts
 *  verändert.
 *  Die Neuberechnung der Geometriedaten findet nur statt, wenn
 *  sich der Animationsparameter seit der letzten Berechnung
 *  verändert hat.
 *
 *  Parameter:
 *    vertexbuffer
 *       ID des Vertex Buffer Objects, das die Raumpunkte
 *       (Vertices) der Raumfläche aufnimmt.
 *    normalenbuffer
 *       ID des Vertex Buffer Objects, das die Normalenvektoren
 *       aufnimmt.
 */
void ApplicationModel::computeVariableGeometryData
           (GLuint vertexbuffer, GLuint normalenbuffer) {
    if (parameterUpdated) {
        computeVerticesAndNormals(c_nx, c_ny, 1.6f, 5.0f, -M_PI, M_PI);

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
    }
}


/*
 *  Die Methode berechnet die Geometriedaten einer Raumfläche,
 *  die durch eine reellwertige Funktion f(r, t) zweier reeller
 *  Parameter r und t definiert ist.
 *  
 *  Aufrufparameter:
 *  nx       Anzahl der Stützstellen für den Parameter r.
 *  ny       Anzahl der Stützstellen für den Parameter t.
 *  x0       Anfangswert des Parameters r der Raumkurve.
 *  xStop    Endwert des Parameters r der Raumkurve.
 *  y0       Anfangswert des Parameters t der raumkurve.
 *  yStop    Endwert des Parameters t der Raumkurve.
 *  Die Stützstellen beider Parameter werden äquvidistant
 *  gewählt.
 */
void ApplicationModel::computeVerticesAndNormals
                 (int nx, int ny,
                  float x0, float xStop,
                  float y0, float yStop) {
    float stepPx = (xStop - x0)/(nx - 1);
    float stepPy = (yStop - y0)/(ny - 1);
    VectorTriple triple;
    
    SpacialPoint sp, spNormalized;
 
    GLfloat* vertexPtr = vertices;
    GLfloat* vertexNormalsPtr = vertexNormals;
       
    for (int i = 0; i < nx; i++) {
        float x = x0 + i*stepPx;
        for (int j = 0; j < ny; j++) {
            float y = y0 + j*stepPy;
             fnp(x, y, &triple);
            *vertexPtr++ = triple.fn.getX();
            *vertexPtr++ = triple.fn.getY();
            *vertexPtr++ = triple.fn.getZ();
            SpacialPoint n = triple.fdx.cross(triple.fdy);
            n = n.safelyNormalized();
            *vertexNormalsPtr++ = n.getX();
            *vertexNormalsPtr++ = n.getY();
            *vertexNormalsPtr++ = n.getZ();
        }
    }
}

/*
 *  Die Methode berechnet die Texturkoordinaten und die
 *  Indices des Gitters
 *  Aufrufparameter:
 *  nx       Anzahl der Stützstellen für den Parameter r.
 *  ny       Anzahl der Stützstellen für den Parameter t.
 *  indices  Zeiger auf ein Feld, das die Indices des
 *           Gitters aufnimmt.
 *  textureKoordinaten
 *           Zeiger auf ein Feld, das die Texturkoordinaten
 *           aufnimmt. Es folgen immer der u-Wert und der
 *           v-Wert aufeinander.
 *
 *  Für die Berechnung der Texturkoordinaten werden Inkrementwerte
 *  von 0.5 verwendet. Damit werden Texturkoordinaten für die
 *  wiederholte Verwendung des im Bereich [0, 1] x [0, 1]
 *  vorbereiteten Musters erhalten. Es entsteht ein Schachbrett-
 *  muster. 
 */
void ApplicationModel::createInvariantGeometryData
                 (int nx, int ny,
                  GLushort *indices, GLfloat *textureCoordinates 
                 ) {
    GLfloat* texCoordPtr = textureCoordinates;
    const float deltaX = 0.5f,
                deltaY = 0.5f;
       
    for (int i = 0; i < c_nx; i++) {
        float tx = i * deltaX;
        for (int j = 0; j < c_ny; j++) {
            float ty = j * deltaY;
            *texCoordPtr++ = tx;
            *texCoordPtr++ = ty;
        }
    }
    //  Berechnung der Mesh-Indices
    GLushort *indexPtr = indices; 
    for (int i = 0; i < ny - 1; i++) {
        for (int j = 0; j < nx - 1; j++) {
            unsigned int k = (unsigned int)(j*ny + i);
            *indexPtr++ = k;
            *indexPtr++ = k + ny;
            *indexPtr++ = k + 1;
            *indexPtr++ = k + ny;
            *indexPtr++ = k + ny + 1;
            *indexPtr++ = k + 1;
        }
    }
}

/*
 *  Berechnung des Funktionswerts und der beiden partiellen
 *  Ableitungen der parametrisierten Funktion.
 *  Die Funktion definiert eine als Plückerfläche bekannte
 *  Raumfläche.
 *
 *  Die Parameter bedeuten:
 *    r   einen Radius, den in die x-y-Ebene projizierten Abstand
 *        vom Mittelpunkt der Rotationsfläche. r sollte nicht
 *        negativ gewählt werden. 
 *    t   einen Rotationswinkel. Der empfohlene Wertebereich
 *        dieses Parameters ist das Intervall [-pi, pi].
 *
 *  Der als Instanzvariable bereitgestellte Animationsparameter
 *  counter, der ganzzahlige Werte zwischen 0 und 359 annimmt,
 *  wird im Argument einer trigonometrischen Funktion verwendet,
 *  die die Höhe der Plückerfläche periodisch vergrößert und
 *  verkleinert. 
 *
 *  Das Ergebnis der Rechnung wird über den Referenzparameter  ft
 *  zurückgegeben, der die Werte dreier Raumpunkte aufnimmt.
 */
void ApplicationModel::fnp (float r, float t, VectorTriple *ft) {
    //  Berechnung der veränderlichen Höhe der Raumfläche:
    float arg = (float)counter/180.0*M_PI;
    float h = hoehe/2.0* (1.1 + sin(arg));
    ///  Für den Wert von  h  könnten man sich auch andere
    ///  Funktionsausdrücke überlegen. Beispiele geeigneter
    ///  Funktionsausdrücke sind:
    ///    float h = hoehe * (1.0 + sin(arg));
    ///    float h = hoehe * sin(arg);

    //  Funktionswert an der Stelle (r, t):
    ft -> fn = SpacialPoint(r*cos(t),
                            r*sin(t),
                            h * sin(blaetter*t));

    //  Wert der partiellen Ableitung nach r an der Stelle (r, t):
    ft -> fdx = SpacialPoint(cos(t),
                             sin(t),
                             0.0f);

    //  Wert der partiellen Ableitung nach t an der Stelle (r, t):
    ft -> fdy = SpacialPoint(-r*sin(t),
                              r*cos(t),
                              h*blaetter*cos(blaetter*t));
}
