#include "main.h"

using namespace std;




/*
 * CONSTRUCTEUR
 */
Astre::Astre() {
    a_pos.x = 0;
    a_pos.y = 0;
    a_vit.x = 0;
    a_vit.y = 0;
    a_masseMantisse = a_masseExposant  = 0;
    a_diam = 0;
    a_clr = Couleur(255,255,255);
    nom = "inconnu";
}

Astre::Astre(string Nom, Vector2f pos, Vector2f vit, 
	    float masseMantisse, int masseExposant, 
	    float diam, Couleur clr) {
    a_pos.x = pos.x;
    a_pos.y = pos.y;
    a_vit.x = vit.x;
    a_vit.y = vit.y;
    a_masseMantisse = masseMantisse;
    a_masseExposant = masseExposant;
    a_diam = diam;
    a_clr = clr;
    nom = Nom;
}




/*
 * METHODES PUBLIQUES
 */
// ajouter à la position
void Astre::AddPosition(float x, float y) {
    a_pos.x += x;
    a_pos.y += y;
}

// ajouter à la vitesse
void Astre::AddVit(float x, float y) {
    a_vit.x += x;
    a_vit.y += y;
}







/*
 * ACCESSEURS
 */
string Astre::getNom() {
    return nom;
}
void Astre::setNom(string s) {
    nom = s;
}
float Astre::getMasse() {
    return a_masseMantisse * pow(10,a_masseExposant);
}
void Astre::setMasseMantisse(float m) {
    a_masseMantisse = m;
}
float Astre::getMasseMantisse() {
    return a_masseMantisse;
}
void Astre::setMasseExposant(int m) {
    a_masseExposant = m;
}
int Astre::getMasseExposant() {
    return a_masseExposant;
}

void Astre::setDiametre(float d) {
    a_diam = fabs(d);
}
float Astre::getDiametre() {
    return a_diam;
}
Couleur Astre::getCouleur() {
    return a_clr;
} 
void Astre::setCouleur(Couleur c) {
    a_clr = c;
}

// Position
void Astre::setPosition(Vector2f p) {
    a_pos.x = p.x;
    a_pos.y = p.y;
}
Vector2f Astre::getPosition() {
    return a_pos;
}



// Vitesse
void Astre::setVitesse(Vector2f v) {
    a_vit.x = v.x;
    a_vit.y = v.y;
}
Vector2f Astre::getVitesse() {
    return a_vit;
}






