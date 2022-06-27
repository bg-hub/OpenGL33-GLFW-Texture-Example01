#include "Texture.hpp"
#include "OpenGLErrorHandler.hpp"
#include <iostream>


Texture::Texture(int w, int h, GLint f, GLubyte *pixels) :
    c_textureWidth(w),
    c_textureHeight(h),
    format(f),
    textureBuffer(pixels)
{
}

Texture::~Texture() {
    delete [] textureBuffer;
    std::cout << "Textur wurde freigegeben" << std::endl;
} 


/*
 *  Herstellung einer Grauwerttextur 
 *  als RGB-Textur ohne Alpha-Kanal
 */
Texture Texture::createRGBTexture(int w, int h, GLubyte bright, GLubyte dark) {
    int c_textureWidth = w;
    int c_textureHeight = h;
    GLubyte *textureBuffer = new GLubyte[3*c_textureWidth*c_textureHeight];  //  für eine RGB-Textur

    GLubyte* textureBufferPtr = textureBuffer;
    
    for (int i = 0; i < c_textureWidth; i++) {
        for (int j = 0; j < c_textureHeight; j++) {
            GLubyte colorToUse;
            if ((i <c_textureWidth/2 && j < c_textureHeight/2)
               || (i >= c_textureWidth/2 && j >= c_textureHeight/2)) {
                   colorToUse = bright;
               } else {
                   colorToUse = dark;
               }
            *textureBufferPtr++ = colorToUse;
            *textureBufferPtr++ = colorToUse;
            *textureBufferPtr++ = colorToUse;
        }
    }
    return Texture (c_textureWidth, c_textureHeight,
                    GL_RGB,
                    textureBuffer);
}



/**
 *  Die Bitmap der Textur wird an die GPU übertragen. Überdies
 *  werden Texturparameter für die Wiederholung der Textur (tiling)
 *  übertragen.
 */
void Texture::installTexture () {

    glTexImage2D(GL_TEXTURE_2D,
                 0,                //  level = 0, weil keine Mipmaps bereitgestellt werden
                 format,
                 c_textureWidth,
                 c_textureHeight,
                 0,                //  border.  Muss 0 sein.
                 format,
                 GL_UNSIGNED_BYTE,
                 textureBuffer);
        OpenGLErrorHandler::checkAndReportErrors
              (typeid(this).name(),
               __func__,
               __LINE__, "glTexImage2D");
    //  Die nachfolgenden Eigenschaften der Textur sind hier fest codiert;
    //  sie schreiben die Wiederholung der Textur (tiling) ohne
    //  Verwendung von Mipmaps vor.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        OpenGLErrorHandler::checkAndReportErrors("glTextParameteri 1");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        OpenGLErrorHandler::checkAndReportErrors("glTextParameteri 2");
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
        OpenGLErrorHandler::checkAndReportErrors("glTextParameteri 3");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);  
        OpenGLErrorHandler::checkAndReportErrors("glTextParameteri 4");
}
