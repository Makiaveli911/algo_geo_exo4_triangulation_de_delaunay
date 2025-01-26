//
// Created by chaid on 13/12/2024.
//
#include "carte.h"
#include "triangulation.h"
#include <algorithm> // POUR LE SORT
#include <stdexcept>

#include "triangulation.h"
#include <algorithm> // pour std::sort
#include <stdexcept>

// Implémentation de la fonction triangulation
void triangulation(std::vector<Point>& T, Carte& C) {
    // Étape 1 : Trier les points par abscisses croissantes
    std::sort(T.begin(), T.end(), [](const Point& a, const Point& b) {
        return (a.x() < b.x() || (a.x() == b.x() && a.y() < b.y()));
    });

    // Vérification
    if (T.size() < 2) {
        throw std::invalid_argument("L'ensemble de points doit contenir au moins deux points !");
    }

    // Étape 2 : Construire un premier segment de base entre les deux premiers points
    DemiCote* demiCoteReference = C.ajouteCote(T[0], T[1]);
    C.changeDemiCoteParticulier(demiCoteReference);

    // Étape 3 : Ajouter les points restants à l'enveloppe convexe
    for (size_t i = 2; i < T.size(); ++i) {
        Point pointAAjouter = T[i];

        DemiCote* demiCoteReference2 = C.ajouteCote(pointAAjouter,demiCoteReference);

        // Sens trigonométrique : trouver les côtés à droite du point

        auto p1 = demiCoteReference->suivant();
        auto p2 = demiCoteReference->suivant()->suivant();

        while (pointAAjouter.aGauche(demiCoteReference->point(), demiCoteReference->suivant()->suivant()->oppose()->point()) == -1) {
            demiCoteReference = demiCoteReference->suivant()->suivant()->oppose();
            C.ajouteCote(demiCoteReference, demiCoteReference2);
        }

        demiCoteReference = demiCoteReference2->oppose()->precedent();

        while(pointAAjouter.aGauche(demiCoteReference->oppose()->point(), demiCoteReference->point()) == -1) {
            demiCoteReference = demiCoteReference->oppose()->precedent();
            demiCoteReference2 = C.ajouteCote(demiCoteReference2, demiCoteReference);
        }

        demiCoteReference = demiCoteReference2;

        // Relier le pointAAjouter aux deux sommets valides
        //DemiCote* nouveauDemiCote = C.ajouteCote(sommetReference->oppose(), pointAAjouter);
    }
}