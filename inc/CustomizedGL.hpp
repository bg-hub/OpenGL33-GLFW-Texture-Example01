#ifndef _CUSTOMIZED_GL_HPP_
#define _CUSTOMIZED_GL_HPP_

/*
 *  Diese Headerdatei erlaubt den einfachen Wechsel zwischen
 *  verschiedenen Profilen von OpenGL. Es kommen die folgenden,
 *  im Verzeichnis  /usr/include/GL  vorgehaltenen Dateien in
 *  Betracht:
 *      glcorearb.h
 *         das aktuellste core profile von OpenGL - auf dem
 *         Jetson Nano gegenwärtig  OpenGL 4.6 Core Profile.
 *      gl.h
 *         ein Kampatibilitätsprofil, das nur die Definitionen
 *         von OpenGL 1.x bereitstellt, falls das Token
 *         GL_GLEXT_LEGACY  definiert ist. Andernfalls werden
 *         zusätzlich zu den Definitionen von OpenGL 1.x noch
 *         die Definitionen von  glext.h  bereitgestellt.
 *      glext.h
 *         ein Kompatibilitätsprofil, das alle jemals für
 *         irgendeine Version von OpenGL 1.x bereitgestellten
 *         Erweiterungen umfasst.
 *  Es darf immer nur eine dieser drei Headerdateien verwendet
 *  werden.
 */
#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>


#endif
