#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm.h>
#include <gtkmm/window.h>
#include "ApplicationModel.hpp"
#include "OpenGLSceneView.hpp"

///  Wenn Gtk-3 mindestens Version 3.24 ist, sollte die nachfolgende
///  Definition verwendet werden; andernfalls ist sie auszukommentieren:
//#define USE_TICK_CALLBACK

/*
 *  Eine Instanz von  MainWindow  agiert als Deskriptor des
 *  Hauptfensters der Anwendung. 
 *  Für die Datenversorgung des Hauptfensters werden Werte aus
 *  einer Instanz von  ApplicationModell  herangezogen. 
 */
class MainWindow :  public Gtk::Window  {

    public:
        MainWindow(Glib::RefPtr<Gtk::Application> app, ApplicationModel &m);
        virtual ~MainWindow();

    protected:
        bool onDeleteEvent(GdkEventAny *event);
        
    private:
        ApplicationModel &model;
        Gtk::Box          vbox;
        OpenGLSceneView   paintArea;
#ifdef USE_TICK_CALLBACK
        guint   animatorId;
        bool onTickEvent(const Glib::RefPtr<Gdk::FrameClock>& frame_clock);  //  erst ab Version 3.24
#else
        sigc::connection animatorId;
        bool on_timeout();
#endif
};

#endif
