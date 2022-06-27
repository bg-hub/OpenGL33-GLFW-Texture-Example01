#version 330
precision mediump float;

/**
 *   Dieser Fragment Shader kombiniert die durch Beleuchtung
 *   erzeugte Farbe eines Fragments mit der durch die
 *   Texturkoordinate gegebenen Farbe der Textur.
 *   Diese Art der Kombination entspricht in OpenGL ES 1.1
 *   der Festlegung
 *      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 */

uniform sampler2D texture;
in vec2 texCoordinate;
uniform bool useTexture;

in vec4 fragmentColor;

void main() {
   if (useTexture) {
       gl_FragColor = texture2D(texture, texCoordinate) * fragmentColor;
   } else {
       gl_FragColor = fragmentColor;
   }
}
