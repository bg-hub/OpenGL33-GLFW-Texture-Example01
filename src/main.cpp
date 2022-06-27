#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "ApplicationModel.hpp"
#include "ShaderInterface.hpp"

//  Das Modell wird hier vereinbart, damit es den Callback-Funktionen
//  sichtbar ist.
ApplicationModel model;
ShaderInterface shaderinterface;


/*
 *  Fehlerbehandlungsfunktion für GLFW.
 *  =>  https://www.glfw.org/docs/3.3/quick_guide.html
 */
void errorCallback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

/*
 *  Diese Ereignisbehandlungsfunktion wird aufgerufen, wenn
 *  sich die Größe des Framebuffer ändert. In dieser Situation
 *  ist der Viewport von OpenGL neu festzulegen. 
 *  Die Festlegung des Viewports geschieht in
 *  ShaderInterface>>renderImage  unter Verwendung der in
 *  model  bereitgestellten Daten   screen_width, screen_height,
 *  image_width. 
 */
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    int s = width < height ? width : height;
    model.screen_width = width;
    model.screen_height = height;
    model.image_width = s;
}

/*
 *  Diese Ereignisbehandlungsfunktion wird aufgerufen, um
 *  Tastatureingaben zu verarbeiten.
 *  In diesem Beispiel wird die Verwendung der Textur mit
 *  einer Tastatureingabe ein- und ausgeschaltet. Außerdem
 *  kann durch Tastatureingabe zwischen einer Parallelprojektion
 *  und einer Zentralprojektion gewählt werden.
 * 
 *  Das  Umschalten der Texturanzeige geschieht durch Eingabe
 *  des Buchstaben T. 
 *  Das  Umschalten der Projektion geschieht durch Eingabe
 *  des Buchstaben P.
 *  Auf Groß- und Kleinschreibung kommt es dabei nicht an.
 */
void keyboardInputCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        model.toggleTexture();
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        model.toggleProjection();
}


/*
 *  Die Funktion stellt ein GLFW-Fenster her und stattet es
 *  mit einem für OpenGL 3.3 konfigurierten Grafikkontext aus.
 *  Das Fenster wird für mittige Platzierung auf dem Bildschirm
 *  vorbereitet, aber nicht sichtbar gemacht.
 *
 *  Der Rückgabewert der Funktion ist ein Verweis auf das
 *  erzeugte Fenster, oder der Wert  NULL,  falls es nicht
 *  möglich war, das Fenster zu erzeugen.
 */
GLFWwindow *createWindow(ApplicationModel *model) {
    const int wndWidth  = 600,
              wndHeight = 600;
    GLFWwindow *wnd;

    //  Ermittlung der Auflösung des Monitors. Die erhaltenen
    //  Daten werden für die mittige Platzierung des Fensters
    //  verwendet.
    int monWidth, monHeight;
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    monWidth = mode->width;
    monHeight = mode->height;

    //  Einen Grafikkontext für OpenGL 3.3 konfigurieren
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API); 

    //  Eigenschaften des Grafikkontexts festlegen
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SAMPLES, 1);

    //  weitere Eigenschaften des Fensters
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    //  Herstellung des Fensters
    wnd = glfwCreateWindow
           (wndWidth,             //  anfängliche Breite
            wndHeight,            //  anfängliche Höhe
            "OpenGL 3.3 Demo",    //  Titel
            NULL,                 //  Monitor
            NULL);                //  kein Context Sharing 
    if (wnd == NULL) {
        return NULL;
    }

    //  Größe des Framebuffers ermitteln und in das Datenmodell
    //  eintragen. Die Werte werden für die Festlegung des
    //  Viewport benötigt.
    int width, height;
    glfwGetFramebufferSize(wnd, &width, &height);
    int s = width < height ? width : height;
    model -> screen_width = width;
    model -> screen_height = height;
    model -> image_width = s;

    //  minimale Fenstergröße festlegen
    glfwSetWindowSizeLimits(wnd,
                            200,             //  min width
                            200,             //  min height
                            GLFW_DONT_CARE,  //  max width
                            GLFW_DONT_CARE); //  max height

    //  Fenster mittig platzieren
    glfwSetWindowPos(wnd, (monWidth - wndWidth)/2,
                          (monHeight - wndHeight)/2);
      //  aus unklarer Ursache muss  glfwSetWindowPos nach
      //  glfwSetWindowSizeLimits  kommen!
    return wnd;
}


/*
 *  main  ist der Startpunkt des Programms.
 *  Die Funktion  main  stellt das Fenster her, stattet es aus,
 *  macht es sichtbar und realisiert die Ereignisverarbeitung.
 */
int main(int argc, char *argv[])
{   
    //  GLFW für den Gebrauch vorbereiten
    //  Wenn  glad  verwendet werden soll, muss  glad
    //  glad initialisiert werden, bevor  glfw  initialisiert
    //  wird.
    if (!glfwInit())
    {
         return -1;
    } else {
         glfwSetErrorCallback(errorCallback);
    }
    //  Das Fenster erzeugen
    GLFWwindow *wnd = createWindow(&model);
    if (wnd == NULL) {
       std::cout << "Fenster wurde nicht erzeugt" << std::endl;
       return -1;
    }
    glfwMakeContextCurrent(wnd);
    glfwSetFramebufferSizeCallback(wnd, framebufferSizeCallback);
    glfwSetKeyCallback(wnd, keyboardInputCallback);

    //  Shaderprogramm compilieren
    shaderinterface.initShaderInterface(model);

    glfwShowWindow(wnd);   //  Fenster sichtbar machen

    //  Ereignisverarbeitungsschleife
    while (!glfwWindowShouldClose(wnd))
    {
        shaderinterface.renderImage(model);
        glfwSwapBuffers(wnd);
        glfwPollEvents();
        model.advanceAnimationParameter();
    }

    //  Das Shaderprogramm freigeben
    shaderinterface.releaseShaderProgram();

    glfwTerminate();
    return 0;
}












