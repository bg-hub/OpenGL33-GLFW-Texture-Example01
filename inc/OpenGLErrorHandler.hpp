#ifndef _OPENGL_ERROR_HANDLER_HPP_
#define _OPENGL_ERROR_HANDLER_HPP_

#include <typeinfo>   
/*
 *  Die Klasse stellt zwei statische Methoden bereit, die den
 *  Fehlerindikator von OpenGL auslesen und eine Nachricht
 *  ausgeben, falls der Fehlerindikator das Vorliegen eines
 *  Fehlers anzeigt.
 *  Beide Methoden geben den logischen Wert  true  zurück,
 *  falls kein Fehler festgestellt wurde. Falls das Auftreten
 *  eines Fehlers festgestellt wurde, wird der logische Wert
 *  false  zurückgegeben.
 */
class OpenGLErrorHandler {
     
    public:
        static bool checkAndReportErrors(char const *message);
        static bool checkAndReportErrors(char const *className,
                                         char const *fnName,
                                         int lineNro, char const *message);
};


#endif
