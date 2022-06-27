#ifndef _OPEN_GL_SCENE_VIEW_H_
#define _OPEN_GL_SCENE_VIEW_H_

#include <gtkmm/glarea.h>
#include "ShaderInterface.hpp"


/*
 *  Eine Instanz von  OpenGLSceneView  leistet die Versorgung
 *  des Grafikwidgets, in dem die mit OpenGL dargestellte
 *  Szene angezeigt wird.
 */
class OpenGLSceneView  : public Gtk::GLArea  {

  public:
     OpenGLSceneView(ApplicationModel &m);
     virtual void on_realize();
     virtual bool on_render (const Glib::RefPtr< Gdk::GLContext >& context);

  private:

     ApplicationModel  &model;
     ShaderInterface   shaderInterface;
};
#endif
