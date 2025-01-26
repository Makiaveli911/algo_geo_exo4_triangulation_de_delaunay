#include "carte.h"
#include "point.h"


// Destructeur
Carte::~Carte() {
  for (auto *demiCote: tableauDemiCote) {
    delete demiCote;
  }
  for (auto *sommet: tableauSommets) {
    delete sommet;
  }
}

// Ajoute dans la carte un nouveau c�t� compos� de deux demi-c�t�s dc1 et dc2
// dont precedent1 et precedent2 seront les demi-c�t�s pr�c�dents respectifs.
// On suppose precedent1 et precedent2 d�j� pr�sents dans la carte.
// Renvoie l'adresse de dc1.
DemiCote *Carte::ajouteCote(DemiCote *precedent1, DemiCote *precedent2) {
  DemiCote *dc1 = ajouteDemiCote(precedent1, nullptr);
  ajouteDemiCote(precedent2, dc1);
  return dc1;
}

// Ajoute dans la carte un nouveau c�t� compos� de deux demi-c�t�s dc1 et dc2.
// precedent1 sera le demi-c�t� pr�c�dent de dc1. dc2 sera issu d�un
// nouveau sommet (� cr�er) dont les coordonn�es sont celles du point p2.
// On suppose precedent1 d�j� pr�sent dans la carte.
// Renvoie l'adresse de dc1.
DemiCote *Carte::ajouteCote(DemiCote *precedent1, const Point &p2) {
  DemiCote *dc1 = ajouteDemiCote(precedent1, nullptr);
  ajouteDemiCote(p2, dc1);
  return dc1;
}

// Ajoute dans la carte un nouveau c�t� compos� de deux demi-c�t�s dc1 et dc2.
// precedent2 sera le demi-c�t� pr�c�dent de dc2. dc1 sera issu d�un
// nouveau sommet (� cr�er) dont les coordonn�es sont celles du point p1.
// On suppose precedent2 d�j� pr�sent dans la carte.
// Renvoie l'adresse de dc1.
DemiCote *Carte::ajouteCote(const Point &p1, DemiCote *precedent2) {
  DemiCote *dc1 = ajouteDemiCote(p1, nullptr);
  ajouteDemiCote(precedent2, dc1);
  return dc1;
}

// Ajoute dans la carte un nouveau c�t� compos� de deux demi-c�t�s dc1 et dc2.
// dc1 et dc2 seront issus de deux nouveaux sommets (� cr�er) dont les
// coordonn�es sont celles des points p1 et p2 respectivement.
// Renvoie l'adresse de dc1.
DemiCote *Carte::ajouteCote(const Point &p1, const Point &p2) {
  DemiCote *dc1 = ajouteDemiCote(p1, nullptr);
  ajouteDemiCote(p2, dc1);
  return dc1;
}

// Ajoute un nouveau demi-c�t� dans la carte dont precedent sera le demi-c�t�
// 	pr�c�dent et oppose sera le demi-c�t� oppos�.
// On suppose que le demi-c�t� precedent est d�j� pr�sent dans la carte.
// Le demi-c�t� oppose est soit d�j� pr�sent dans la carte soit nul (valeur
// par d�faut).
// Renvoie l�adresse du nouveau demi-c�t�.
DemiCote *Carte::ajouteDemiCote(DemiCote *precedent, DemiCote *oppose) {
  auto *demiCote = new DemiCote(precedent->demiCoteSuivant,
                                precedent,
                                oppose,
                                precedent->demiCoteSommet,
                                tailleTableauDemiCotes());
  tableauDemiCote.push_back(demiCote);

  precedent->demiCoteSuivant = demiCote;
  demiCote->demiCoteSuivant->demiCotePrecedent = demiCote;
  if (oppose != nullptr) {
    oppose->demiCoteOppose = demiCote;
  }
  return demiCote;
}

// Ajoute un nouveau de mi-c�t� dans la carte qui sera issu d�un nouveau
// sommet (� cr�er) dont les coordonn�es sont celles du point p.
// oppose sera le demi-c�t� oppos� du nouveau demi-c�t�.
// oppose est soit d�j� pr�sent dans la carte soit nul (valeur par d�faut).
// Renvoie l�adresse du nouveau demi-c�t�.
DemiCote *Carte::ajouteDemiCote(const Point &p, DemiCote *oppose) {
  auto *sommet = new Sommet(p, nullptr, tailleTableauDemiCotes());
  tableauSommets.push_back(sommet);

  auto *demiCote = new DemiCote(nullptr,
                                nullptr,
                                oppose,
                                sommet,
                                tailleTableauDemiCotes());
  demiCote->demiCoteSuivant = demiCote;
  demiCote->demiCotePrecedent = demiCote;
  tableauDemiCote.push_back(demiCote);

  sommet->demiCote = demiCote;
  if (oppose != nullptr) {
    oppose->demiCoteOppose = demiCote;
  }
  return demiCote;
}


void Carte::flip(DemiCote *d) {
  // On traite le premier demicote
  d->precedent()->demiCoteSuivant = d->suivant();
  d->suivant()->demiCotePrecedent = d->precedent();

  DemiCote *d_initial_oppose = d->precedent()->oppose();
  d->demiCoteSommet = d_initial_oppose->sommet();

  d_initial_oppose->demiCotePrecedent->demiCoteSuivant = d;
  d->demiCotePrecedent = d_initial_oppose->demiCotePrecedent;

  d_initial_oppose->demiCotePrecedent = d;
  d->demiCoteSuivant = d_initial_oppose;

  // On traite son oppos�
  DemiCote *d_oppose = d->oppose();
  d_oppose->precedent()->demiCoteSuivant = d_oppose->suivant();
  d_oppose->suivant()->demiCotePrecedent = d_oppose->precedent();

  DemiCote *d_oppose_oppose = d_oppose->precedent()->oppose();
  d_oppose->demiCoteSommet = d_oppose_oppose->sommet();

  d_oppose_oppose->demiCotePrecedent->demiCoteSuivant = d_oppose;
  d_oppose->demiCotePrecedent = d_oppose_oppose->demiCotePrecedent;

  d_oppose_oppose->demiCotePrecedent = d_oppose;
  d_oppose->demiCoteSuivant = d_oppose_oppose;
}
