#ifndef _LIGHT_PROPERTIES_HPP_
#define _LIGHT_PROPERTIES_HPP_


/*
 *  Die Struktur  LightProperties  fasst alle Werte
 *  zusammen, die für die Beschreibung der Eigenschaften einer
 *  positionierten Lichtquelle benötigt werden.
 *  Die Eigenschafte eines Lichtquelle sind:
 *   -  drei Farbwerte für die drei verschiedenen Arten
 *      von Licht,
 *   -  die Position der Lichtquelle,
 *   -  ihr Zustand (eingeschaltet/ausgeschaltet)
 *   -  die Art ihrer Positionierung (in der Szene
 *   -  oder ausserhalb der Szene)
 *
 *  Die Datenstruktur kann unverändert auch für die Festlegung
 *  der Eigenschaften einer gerichteten Lichtquelle verwendet
 *  werden. Im Beispielprogramm werden aber nur positionierte
 *  Lichtquellen benötigt.
 */
struct LightProperties {
    public:
     float position[4]; 
     float ambientColor[4];
     float diffuseColor[4];
     float specularColor[4];
     bool isOn;
     bool isFixed;
};
#endif
