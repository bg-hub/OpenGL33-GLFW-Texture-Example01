#version 330
precision mediump float;

/**
 *   Dies ist ein sehr spezieller Shader, der zwei 
 *   platzierte Lichtquellen ohne Lichtabschwächung
 *   unterstützt.
 *   2020-09-27
 */

//  Attribute (Geometriedaten eines Vertex)
layout(location = 0) in  vec3 position;
layout(location = 1) in  vec3 normals;
layout(location = 2) in  vec2 textureCoordinate;

out vec2 texCoordinate;
out vec4 fragmentColor;

//
//  Datenstrukturen
//

struct materialProperties {
    vec4   ambientColor;
    vec4   diffuseColor;
    vec4   specularColor;
    vec4   emissiveColor;
    float  specularExponent;  // shininess
   };
  
/**
 *  Diese sind die Parameter für Lichtquellen ohne
 *  Lichtabschwächung (Attenuation).
 */
struct lightProperties {
    vec4   ambientColor;
    vec4   diffuseColor;
    vec4   specularColor;
    vec4   position; 
    bool   isFixed;
    bool   isOn;
};

//  häufig benötigte Konstanten:
const float fZero = 0.0;
const float fOne  = 1.0;

//
//   Variablen des Shaders:
//

uniform bool   useTexture;

uniform materialProperties  material;
uniform lightProperties     light0;
uniform lightProperties     light1;

uniform vec4      ambientSceneLight;
uniform bool      enableLighting;

uniform mat4      mvp;
uniform mat4      modelView;
uniform mat3      invModelView;


//  Hilfsvariablen des Vertexshaders:
vec3     n;
vec4     matAmbientColor;
vec4     matDiffuseColor;
    
vec4     v_front_color;

/**
 *   Berechnung der Wirkung der Lichtquelle  lightSource
 */
vec4 computeLighting(in vec4 eyePos, in lightProperties lightSource) {
    vec4 computedColor;
    vec3 hVec;
    vec3 VPpli;
    float nDotL;
    float nDotH;
    computedColor = vec4(fZero, fZero, fZero, fZero);

    if (lightSource.isFixed) {
        VPpli = lightSource.position.xyz;
    } else {
        VPpli = (modelView*lightSource.position).xyz;
    }
    if (lightSource.isOn && (lightSource.position.w != fZero)) {  // positional light
        VPpli -= eyePos.xyz;
        if (length(VPpli) > 0.0) {
            VPpli = normalize(VPpli);
        }
    }

    if (lightSource.isOn) {
        computedColor += (lightSource.ambientColor*matAmbientColor);
        nDotL = max(fZero, dot(n, VPpli));
        computedColor += (nDotL*lightSource.diffuseColor*matDiffuseColor);
        hVec = normalize(VPpli + vec3(fZero, fZero, fOne));
        nDotH = dot(n, hVec);
        if (nDotH > fZero) {
            computedColor +=(pow(nDotH, material.specularExponent) *
                               material.specularColor*
                               lightSource.specularColor);
        }
    }
    return computedColor;
}

/**
 *
 */
void main() {
    vec4     eyePos;
    
    eyePos = modelView * vec4(position, 1.0);
    if (enableLighting)
    {
        n = invModelView * normals;
        n = normalize(n);

        matAmbientColor = material.ambientColor; 
        matDiffuseColor = material.diffuseColor; 
        
        v_front_color = material.emissiveColor +
                        (matAmbientColor*ambientSceneLight); 
        v_front_color = clamp(v_front_color, fZero, fOne);

        v_front_color += computeLighting(eyePos, light0);
        v_front_color += computeLighting(eyePos, light1); 
 
        v_front_color.a = matDiffuseColor.a;
    }
    if (!enableLighting) {
        v_front_color = material.ambientColor;
    }
    if (useTexture) {
        texCoordinate = textureCoordinate;
    }
    
   gl_Position = mvp*vec4(position, 1.0);
   fragmentColor =  clamp(v_front_color, fZero, fOne);
}
