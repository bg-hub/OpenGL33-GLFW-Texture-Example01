Cpp-GLFW-GL33-Scene01

   +-------------------------------------------------------+
   |   Dieses Programm wird für den Rechner  Jetson Nano   |
   |   bereitgestellt. Die Verwendung des Programms auf    |
   |   ähnlichen Rechnern könnte möglich sein, falls die   |
   |   im Makefile genannten Programmbibliotheken          |
   |   vorhanden sind.                                     |
   +-------------------------------------------------------+

Hilfmittel:
   GLFW, GLM, OpenGL 3.3 Core Profil.

Erforderliche Pakete:

   sudo apt-get install libglfw3-dev
   sudo apt-get install libglm-dev libglm-doc

Das Beispiel zeigt eine rotierende, formveränderliche, doppelseitig
gefärbte Raumfläche. Durch Eingabe des Buchstabens "T" kann Texturierung
der Raumfläche eingeschaltet oder ausgeschaltet werden. Durch Eingabe
des Buchstabens "P" kann zwiwchen einer Parallelprojektion und einer
Zentralprojektion umgeschaltet werden.


Dieses in C++ geschriebene Programm verwendet OpenGL 3.3 Core Profile
mit GLFW. Für die Berechnung der Projektionsmatrix werden Hilfsmittel
aus GLM verwendet.

Die Shader werden aus Dateien gelesen, die im Verzeichnis ./res
bereitgestellt werden. Die Versorgung des Shaderprogramms ist 
in einem separaten Modul implementiert.



Verwendung des Makefiles
------------------------

Compilierung:
   make

Programmausführung:
    ./demo

Löschung aller vom Compiler erzeugten Programme:
   make clean



Internet-Resourcen zu GLFW:
---------------------------

  Homepage:
     https://www.glfw.org/

  Einführung:
     https://www.glfw.org/docs/latest/intro_guide.html

  Tutorial:
     https://www.glfw.org/docs/latest/quick_guide.html



letzte Revision: 2022-06-15
