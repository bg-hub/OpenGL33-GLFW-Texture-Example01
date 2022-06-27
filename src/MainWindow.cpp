#include "MainWindow.hpp"
//#include <stdio>

#define FRAME_DELAY 8
 //6  //  für Raspberry: 20

/*
 *  Der Konstruktor stattet das Hauptfenster aus, macht es
 *  sichtbar und startet den für die Animation verwendeten
 *  Zeitgeber.
 */
MainWindow::MainWindow(Glib::RefPtr<Gtk::Application> app,
                       ApplicationModel &m) :
   model(m),
   paintArea(m)
{
    set_title("OpenGL 3.3 Demo");
    set_reallocate_redraws(true); 
    set_default_size(500, 520);   //  anfängliche Größe des Fensters
    set_size_request(300, 320);   //  Mindestgröße des Fensters
    set_position(Gtk::WIN_POS_CENTER);
    signal_delete_event().
         connect(sigc::mem_fun(*this, &MainWindow::onDeleteEvent));
    add(vbox);
    
    vbox.pack_start(paintArea);
    paintArea.show();
    vbox.show();

    //  Animation starten:
#ifdef USE_TICK_CALLBACK
    //  in der Version 3.24 von Gtkmm-3 ist es
    //  möglich, add_tick_callback zu verwenden: 
     animatorId = paintArea.add_tick_callback
                (sigc::mem_fun(*this, &MainWindow::onTickEvent));
#else
    animatorId = Glib::signal_timeout().connect(
              sigc::mem_fun(*this, &MainWindow::on_timeout), FRAME_DELAY);
#endif
    show();
}

/*
 *  Der Destruktor deaktiviert den mit dem Fenster verbundenen
 *  Zeitgeber.
 */
MainWindow::~MainWindow() {
    animatorId.disconnect();
}

/*
 *  Die Callback-Funktion für das Ereignis  delete_event
 *  genehmigt as Schliessen des Fensters.
 */
bool MainWindow::onDeleteEvent(GdkEventAny *event) {
    return false;  // Weiterbehandlung zulassen
};


/*
 * Diese Ereignisbehandlungsfunktion wird durch einen Timer ausgelöst.
 * Sie erzeugt die Animation der Grafik, indem sie den Phasenparameter
 * und den Rotationswinkel des Diagramms verändert und die Grafik neu
 * berechnet und darstellt.
 */
#ifdef USE_TICK_CALLBACK
bool MainWindow::onTickEvent(const Glib::RefPtr<Gdk::FrameClock>& frame_clock) {
#else
bool MainWindow::on_timeout() {
#endif
    model.advanceAnimationParameter();
    paintArea.queue_render();

    return  G_SOURCE_CONTINUE;  // Funktion beibehalten
}
