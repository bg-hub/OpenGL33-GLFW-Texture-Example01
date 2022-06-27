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

uniform sampler2D textureSampler;
in vec2 texCoordinate;
uniform bool useTexture;

in vec4 fragmentColor;
out vec4 outFragColor;

void main() {
   if (useTexture) {
       outFragColor = texture(textureSampler, texCoordinate) * fragmentColor;
   } else {
       outFragColor = fragmentColor;
   }
}
