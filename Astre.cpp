#include "main.h"

using namespace sf;




/*
 * CONSTRUCTEUR
 */
// nom coordonnées du centre, masse, diamètre, couleur, vitesse, accélération.
Astre::Astre(Univers *u, String Nom, float x, float y, float masse, float diam, Color clr, float vx, float vy, float ax, float ay, bool aff) {
    U = u;
    a_pos.x = x;
    a_pos.y = y;
    a_vit.x = vx;
    a_vit.y = vy;
    a_acc.x = ax;
    a_acc.y = ay;
    affichage = aff;
    a_masse = masse;
    a_diam = diam;
    a_clr = clr;
    a_bclr = clr;
    estSelectionne = false;
    // création du shape
    shape = Shape::Circle(x, y, diam/2.0, clr, 0.1, clr);
    nom = Nom;
    nom.SetPosition(a_pos.x + a_diam + 5, a_pos.y);
}




/*
 * METHODES PUBLIQUES
 */
// calcule les accélérations en fonction des astres envoyés en arguments
void Astre::calculsdt(std::vector<Astre*> astres) {
    // initialisations
    float distX = 0, distY = 0, dist2D = 0;
    float force2D = 0;
    float inter1 = 0, inter2 = 0;
    // on remet l'accélération à zéro, ou à la valeur minimale
    a_acc.x = gainAcc.x;
    a_acc.y = gainAcc.y;
    gainAcc.x = gainAcc.y = 0;

    // pour chaque astre, si il ne s'agit pas celui étudié
    for(unsigned int i = 0; i < astres.size(); i++) {
	if(this != astres[i]) {
	    // calcul des distances, signées
		distX = astres[i]->GetPosition().x - a_pos.x;
		distY = astres[i]->GetPosition().y - a_pos.y;
		dist2D = sqrt(carre(distX) + carre(distY));
	    // on en déduit la force en newton, et donc la vitesse totale gagnée
		force2D = U->attraction(a_masse, astres[i]->GetMasse(), dist2D);
	    // Le théorème de thalès nous permet d'annoncer la suite :
		inter1 = ( distX*force2D/dist2D ) / a_masse;
		inter2 = ( distY*force2D/dist2D ) / a_masse;
		a_acc.x += inter1;
		a_acc.y += inter2;
	}
    }
}




// modifie sa vitesse selon accélérations, puis sa position en fonction des vitesses
void Astre::Move() {
    AddVit(a_acc.x, a_acc.y); // ajout de l'accélération à la vitesse
    U->borneVitesse(&a_vit);
    AddPosition(a_vit.x, a_vit.y); // ajout de la vitesse à la position
}


// ajouter àa la position
void Astre::AddPosition(float x, float y) {
    a_pos.x += x;
    a_pos.y += y;
}

// ajouter à la vitesse
void Astre::AddVit(float x, float y) {
    a_vit.x += x;
    a_vit.y += y;
}

// ajouter à l'accélération
void Astre::AddAcc(float x, float y) {
    gainAcc.x += x;
    gainAcc.y += y;
}




/*
 * ACCESSEURS
 */
// Généraux
Shape Astre::GetShape() {
    float epaisseurBord = 0;
    if(estSelectionne) epaisseurBord = 5;
    // génération du shape
    shape = Shape::Circle(a_pos.x, a_pos.y, a_diam/2.0, a_clr, epaisseurBord, a_bclr);
    return shape;
}
String Astre::GetNom() {
    nom.SetPosition(a_pos.x + a_diam + 5, a_pos.y);
    return nom;
}
void Astre::SetMasse(float m) {
    a_masse = fabs(m);
}
float Astre::GetMasse() {
    return a_masse;
}

void Astre::SetDiametre(float d) {
    a_diam = fabs(d);
}
float Astre::GetDiametre() {
    return a_diam;
}

void Astre::SetSelection(bool s) {
    estSelectionne = s;
}
bool Astre::GetSelection() {
    return estSelectionne;
}



// Position
void Astre::SetPosition(float x, float y) {
    a_pos.x = x;
    a_pos.y = y;
}
sf::Vector2f Astre::GetPosition() {
    Vector2f inter(a_pos.x, a_pos.y);
    return inter;
}



// Vitesse
void Astre::SetVitesse(float x, float y) {
    a_vit.x = x;
    a_vit.y = y;
}
sf::Vector2f Astre::GetVitesse() {
    Vector2f inter(a_vit.x, a_vit.y);
    return inter;
}


// Acceleration
void Astre::SetAcceleration(float x, float y) {
    a_acc.x = x;
    a_acc.y = y;
}
sf::Vector2f Astre::GetAcceleration() {
    Vector2f inter(a_acc.x, a_acc.y);
    return inter;
}



