#ifndef _DISPLAY_RECTANGLE_HPP_
#define _DISPLAY_RECTANGLE_HPP_

/*
 *  Die Struktur  DisplayRect  stellt für eine Instanz von
 *  ShaderInterface  die Daten bereit, die für die Festlegung
 *  des OpenGL Viewport  benötigt werden.
 *  Die Bereitstellung der Daten geschieht in
 *  OpenGLSceneView>>on_render().
 */
struct DisplayRect {
    public:
        int    screenWidth,
               screenHeight,
               imageWidth;
};
#endif