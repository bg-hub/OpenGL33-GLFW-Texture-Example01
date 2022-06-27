#include <gtkmm.h>
#include "MainWindow.hpp"

Glib::RefPtr<Gtk::Application> app;

int main(int  argc, char *argv[]) {
    Glib::RefPtr<Gtk::Application> app =
         Gtk::Application::create(argc, argv,
                                  "opengles2demo.app",
                                  Gio::APPLICATION_NON_UNIQUE);
    ApplicationModel model(app);
    MainWindow window(app, model);
    app->run(window);
    return 0;
}
