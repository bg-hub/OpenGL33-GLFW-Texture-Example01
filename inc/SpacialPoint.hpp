#ifndef _SPACIAL_POINT_HPP_
#define _SPACIAL_POINT_HPP_
#include <glm/glm.hpp>
#include <iostream>

/*
 *  Eine Instanz stellt einen Raumpunkt durch Angabe seiner
 *  drei Koordiaten in einem orthonormalen
 *  Koordinatensystem dar.
 *
 *  Die arithmetischen Operationen mit Raumpunkten stehen
 *  als Operatoren zur Verfügung.
 *  Überdies stehen Vektoroperationen und ein Ausgabeoperator
 *  zur Verfügung.
 */

class SpacialPoint  {

public:
   SpacialPoint();  // Dieser Konstruktor wird gebraucht, wenn
                    // Variablen vom Typ  SpacialPoint  definiert
                    // werden, für die keine Anfangswerte
                    // angegeben werden.
   SpacialPoint(float px, float py, float pz);
   float getX() const;
   float getY() const;
   float getZ() const;
   float euclideanNorm();
   float dot(SpacialPoint);
   SpacialPoint safelyNormalized();
   SpacialPoint min(SpacialPoint pt);
   SpacialPoint max(SpacialPoint);
   SpacialPoint cross(SpacialPoint);
   glm::vec3 asVec3();
   
   
   SpacialPoint operator - ();
   friend SpacialPoint operator + (SpacialPoint sp1, SpacialPoint sp2);
   friend SpacialPoint operator - (SpacialPoint sp1, SpacialPoint sp2);
   friend SpacialPoint operator * (SpacialPoint sp1, SpacialPoint sp2);
   friend SpacialPoint operator * (SpacialPoint sp1, float factor);
   friend SpacialPoint operator * (float factor, SpacialPoint sp2);
   friend SpacialPoint operator / (SpacialPoint sp1, float d);
   friend std::ostream& operator << (std::ostream& os, const SpacialPoint& pt);

private:
   float x;
   float y;
   float z;

};

#endif
