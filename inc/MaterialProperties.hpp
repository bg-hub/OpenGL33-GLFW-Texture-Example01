#ifndef _MATERIAL_PROPERTIES_HPP_
#define _MATERIAL_PROPERTIES_HPP_


/*
 *  Die Struktur  MaterialProperties  fasst alle Werte
 *  zusammen, die für die Beschreibung der Eigenschaften eines
 *  Materials benötigt werden. In der Terminologie von  OpenGL
 *  bestimmt ein Material die Farbe der Oberfläche eines Szeneobjekts.
 *  Die Eigenschaften eines Materials sind:
 *   -  drei Farbwerte, die auf die drei verschiedenen Arten
 *      von Licht reagieren,
 *   -  ein weiterer Farbwert, der die Leuchtfarbe des
 *      Materials bestimmt, 
 *   -  der Glanzwert (shininess), der festlegt, wie stark 
 *      spiegelndes Licht (specular light) auf das Material
 *      einwirkt.
 */
struct MaterialProperties {
    public:
     float ambientColor[4];
     float diffuseColor[4];
     float specularColor[4];
     float emissiveColor[4]; 
     float shininess;
};
#endif

//  https://www.fluentcpp.com/2018/06/15/should-structs-have-constructors-in-cpp/
