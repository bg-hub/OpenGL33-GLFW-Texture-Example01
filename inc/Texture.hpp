#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <CustomizedGL.hpp>

/*
 *  Eine Instanz der Klasse  Texture  stellt eine Textur
 *  bereit.
 *  Zwei statische Methoden stehen für die Berechnung einer
 *  Textur wählbarer Größe in den Formaten GL_LUMINANCE und
 *  GL_RGB zur Verfügung. Eine Instanzmethode leistet die
 *  Übertragung der Textur an die GPU.
 *
 *  
 */
class Texture {

  public:
    Texture(int w, int h, GLint f, GLubyte *pixels);
    ~Texture();
    void installTexture ();
    static Texture createRGBTexture(int w, int h, GLubyte bright, GLubyte dark);
  private:
    int        c_textureWidth,
               c_textureHeight;
    GLint      format;
    GLubyte   *textureBuffer;
};

#endif
