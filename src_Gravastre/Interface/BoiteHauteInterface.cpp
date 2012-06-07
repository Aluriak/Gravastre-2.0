#include "../main.h"

using namespace sf;



/*
 * CONSTRUCTEUR
 */
BoiteHauteInterface::BoiteHauteInterface(RenderWindow *rw, 
					Font* pol, int tPol,
					Color clr,
					int x, int y,
					int l, int h) {
    // initialisation des attributs
    app = rw;
    AFF_Position.x = x;
    AFF_Position.y = y;
    hauteur = h;
    longueur = l;
    // initialisation des valeurs graphiques
    police = pol;
    taillePolice = tPol;
    // création du shape
    cadre.AddPoint(x,y,clr);
    cadre.AddPoint(x+l,y,clr);
    cadre.AddPoint(x+l,y+h,clr);
    cadre.AddPoint(x,y+h,clr);
}



/*
 * METHODES PUBLIQUES
 */
// Dessine la barre dans app suivant les arguments envoyés
void BoiteHauteInterface::Draw(std::string tampon, std::string directive) {
    app->Draw(cadre);
    String tamp(tampon, *police, taillePolice);
    tamp.SetPosition(	AFF_Position.x + 5*longueur/6.0, 
			AFF_Position.y + 1*hauteur/4.0);
    String direc(directive, *police, taillePolice);
    direc.SetPosition(	AFF_Position.x + 4, 
			AFF_Position.y + 1*hauteur/4.0);
    app->Draw(tamp);
    app->Draw(direc);
}





