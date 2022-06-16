#ifndef _VECTOR_TRIPLE_HPP_
#define _VECTOR_TRIPLE_HPP_

#include "SpacialPoint.hpp"

/*
 *  Die Struktur fasst drei Raumpunkte zusammen, die
 *  situationsabhängig auch als drei Vektoren interpretiert
 *  werden können.
 *
 *  Für eine Funktion f(x, y) bietet sich die folgende, in der
 *  Klasse  ApplicationModel  auch verwendete Interpretation an:
 *    fn    der Wert von f(x, y)
 *    fdx   der Wert der partiellen Ableitung von f(x, y)
 *          nach x an der Stelle x, y
 *    fdy   der Wert der partiellen Ableitung von f(x, y)
 *          nach y an der Stelle x, y
 *  Für die Berechnung eines Normalenvektors werden  fdx, fdy  als
 *  Vektoren interpretiert; das Vektorprodukt dieser Vektoren
 *  liefert gerade einen Normalenvektor, dessen Betrag allerdings
 *  nicht auf 1 normalisiert ist.
 */
struct VectorTriple {
    SpacialPoint fn;
    SpacialPoint fdx;
    SpacialPoint fdy;
};

#endif
