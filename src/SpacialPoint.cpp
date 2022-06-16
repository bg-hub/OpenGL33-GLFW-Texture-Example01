#include "SpacialPoint.hpp"
#include <math.h>

SpacialPoint::SpacialPoint() {
}

SpacialPoint::SpacialPoint (float px, float py, float pz) {
    x = px;
    y = py;
    z = pz;
}

float SpacialPoint::getX() const {
    return x;
}

float SpacialPoint::getY() const {
    return y;
}

float SpacialPoint::getZ() const {
    return z;
}

glm::vec3 SpacialPoint::asVec3() {
    return glm::vec3(x, y, z);
}

std::ostream& operator << (std::ostream& os, const SpacialPoint& pt) {
    os << "SpacialPoint(" << pt.x << ", " << pt.y << ", " << pt.z << ") ";
    return os;
}

SpacialPoint SpacialPoint::operator - () {
    return  SpacialPoint ( - x, - y, - z);
}

SpacialPoint operator + (SpacialPoint sp1, SpacialPoint sp2) {
    return  SpacialPoint (sp1.x + sp2.x, sp1.y + sp2.y, sp1.z + sp2.z);
}

SpacialPoint operator - (SpacialPoint sp1, SpacialPoint sp2) {
    return  SpacialPoint (sp1.x - sp2.x, sp1.y - sp2.y, sp1.z - sp2.z);
}

SpacialPoint operator * (SpacialPoint sp1, SpacialPoint sp2) {
    return  SpacialPoint (sp1.x * sp2.x, sp1.y * sp2.y, sp1.z * sp2.z);
}

SpacialPoint operator * (SpacialPoint sp1, float d) {
    return  SpacialPoint (sp1.x * d, sp1.y * d, sp1.z * d);
}

SpacialPoint operator * (float d, SpacialPoint sp1) {
    return  SpacialPoint (sp1.x * d, sp1.y * d, sp1.z * d);
}

SpacialPoint operator / (SpacialPoint sp1, float d) {
    return  SpacialPoint (sp1.x / d, sp1.y / d, sp1.z / d);
}
   
float SpacialPoint::dot(SpacialPoint pt) {
    return x*pt.x + y*pt.y + z*pt.z;
}

float SpacialPoint::euclideanNorm() {
    float s = x*x + y*y + z*z;
    return sqrt(s);
}

SpacialPoint SpacialPoint::safelyNormalized() {
    float length = euclideanNorm();
    if (length == 1.0f)
        return SpacialPoint(x, y, z);
    if (length != 0.0f)
        return  (*this) / length;
    return SpacialPoint(x, y, z);
}

SpacialPoint SpacialPoint::min(SpacialPoint pt) {
    return SpacialPoint(x < pt.x ? x : pt.x,
                        y < pt.y ? y : pt.y,
                        z < pt.z ? z : pt.z);
}

SpacialPoint SpacialPoint::max(SpacialPoint pt) {
    return SpacialPoint(x > pt.x ? x : pt.x,
                        y > pt.y ? y : pt.y,
                        z > pt.z ? z : pt.z);
}

SpacialPoint SpacialPoint::cross(SpacialPoint pt) {
    float cx =  y*pt.z - z*pt.y;
    float cy = -x*pt.z + z*pt.x;
    float cz =  x*pt.y - y*pt.x;
    return SpacialPoint(cx, cy, cz);
}
