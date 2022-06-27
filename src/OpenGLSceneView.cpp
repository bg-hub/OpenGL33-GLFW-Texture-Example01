#include "OpenGLSceneView.hpp"
#include "CustomizedGL.hpp"
#include "DisplayRectangle.hpp"

/*
 *  Der Konstruktor ...
 */
OpenGLSceneView::OpenGLSceneView(ApplicationModel &m) :
    model(m)
{
    shaderInterface.setVerbose(true);
}


/*
 *  Diese Methode ist in einer der Basisklassen als virtuelle
 *  Methode vereinbart. Sie wird hier reimplementiert, um die
 *  Behandlung des Ereignisses "realize"  zu ermöglichen.
 *  Diese Methode wird einmalig aufgerufen, wenn das Widget
 *  sein GdkWindow erhalten hat.
 *  Die Methode konfiguriert den OpenGL Kontext.
 *  Außerdem werden die Shaderprogramme compiliert und installiert.
 *
 *  Sehr wichtig ist ist der Aufruf der Methode
 *     set_has_depth_buffer
 *  Die Voreinstellung ist, dass kein depth buffer bereitgestellt wird.
 *  Für 3D-Grafik ist der depth buffer aber unentbehrlich.
 */
void OpenGLSceneView::on_realize() {
    Glib::RefPtr< Gdk::GLContext > context;
    this -> set_required_version(3, 3);
    this -> set_has_depth_buffer(true);
    GLArea::on_realize();  //  Aufruf der Methode der Basisklasse

    const bool use_es = this -> get_context()->get_use_es();
    std::cout << "uses es: " << (use_es ? "ja" : "nein") << std::endl;
    this -> get_context() -> make_current();
    //model.computeGeometryData();
    shaderInterface.initShaderInterface(model);
    std::cout << "on_realize was sent" << std::endl;
}



/*
 *  Diese Methode ist in einer der Basisklassen als virtuelle
 *  Methode vereinbart. Sie wird hier reimplementiert, um die
 *  Behandlung des Ereignisses  "render"  zu ermöglichen.
 */
bool OpenGLSceneView::on_render (const Glib::RefPtr< Gdk::GLContext >& context) {
    DisplayRect displayRect;
    int w = get_width();
    int h = get_height();
    int s = w < h ? w : h;
    displayRect.screenWidth = w;
    displayRect.screenHeight = h;
    displayRect.imageWidth = s;
    shaderInterface.renderImage(model, displayRect);
    return true;
}

/*
 *  Diese Methode erzeugt die Befehlsfolge für die GPU und bereitet das
 *  von der GPU erzeugte Bild für die Darstellung im Grafikfenster
 *  auf.
 *
 *  Die eigentliche Bilderzeugung wird durch das Shaderinterface
 *  geleistet. 
 */ /*
void::OpenGLSceneView::redraw_scene(){
    shaderInterface.renderImage(model);
}
*/
