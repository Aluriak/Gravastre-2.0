#include "main.h"

using namespace sf;


/*
 * CONSTRUCTEUR
 */
BoiteInfoAstre::BoiteInfoAstre( RenderWindow *rw, 	// fenetre
				Color clr, 		// couleur du cadre
				float xc, float yc, 	// coord du coin h-g
				int larg, int haut, 	// dimensions
				Font *pol, int tPol	// police
				) {
    // dimension de la boite
    SFML_LargeurBoite = larg;
    SFML_HauteurBoite = haut;
    app = rw;
    // police
    police = pol;
    taillePolice = tPol;
    // couleur
    clrCadre = clr;
    // coin haut-gauche
    AFF_Position.x = xc;
    AFF_Position.y = yc;
    // Création du Shape
    // On créé les quatre points
	cadre.AddPoint(
		AFF_Position.x, AFF_Position.y, 
		clrCadre, clrCadre);
	cadre.AddPoint(
		AFF_Position.x+larg, AFF_Position.y, 
		clrCadre, clrCadre);
	cadre.AddPoint(
		AFF_Position.x+larg, AFF_Position.y+haut, 
		clrCadre, clrCadre);
	cadre.AddPoint(
		AFF_Position.x, AFF_Position.y+haut, 
		clrCadre, clrCadre);
    // Quelques détailes
	cadre.EnableFill(false); // Il n'est pas plein !
	cadre.SetOutlineWidth(1); // large de 10 pixels
}





/*
 * METHODES PUBLIQUES
 */
// affiche tous les objets drawables sur app
void BoiteInfoAstre::Draw(Astre *astre) {
    // CADRE
    // on affiche le cadre
	app->Draw(cadre);

    // DONNEES
    // on initialise les sf::String    
    donnees[0] = astre->GetNom();
	// le nom est mit en plus gros
	donnees[0].SetSize(donnees[0].GetSize()*3);
    donnees[1] = String("Masse : " + float2str(astre->GetMasse()), 
	    *police, taillePolice);
    donnees[2] = String("PositionX : " + float2str(astre->GetPosition().x), 
	    *police, taillePolice);
    donnees[3] = String("PositionY : " + float2str(astre->GetPosition().y), 
	    *police, taillePolice);
    donnees[4] = String("VitesseX : " + float2str(astre->GetVitesse().x), 
	    *police, taillePolice);
    donnees[5] = String("VitesseY : " + float2str(astre->GetVitesse().y), 
	    *police, taillePolice);
    donnees[6] =String("AccelerationX : "+float2str(astre->GetAcceleration().x),
	    *police, taillePolice);
    donnees[7] =String("AccelerationY : "+float2str(astre->GetAcceleration().y),
	    *police, taillePolice);
    
    // on place les différents éléments 
    float coordX = 0, coordY = 0;
    for(int i = 0; i < 8; i++) {
	// coordonnées de base
	coordX = AFF_Position.x + 10;
	coordY = AFF_Position.y + 30 + i*2*taillePolice;
	// corrections pour le titre
	if(i == 0) {
	    coordX += 10; // le nom est centré
	    coordY -= 20; // le nom est relevé
	}
	// application
	donnees[i].SetPosition(coordX, coordY);
	app->Draw(donnees[i]);
    }
}


// Ajoute les valeurs envoyées à la position du coin haut-gauche
void BoiteInfoAstre::addPosition(float x, float y) {
    AFF_Position.x += x;
    AFF_Position.y += y;
    // on modifie les coordonées du cadre
    cadre.SetPointPosition(0, AFF_Position.x, AFF_Position.y);
    cadre.SetPointPosition(1, AFF_Position.x+SFML_LargeurBoite, AFF_Position.y);
    cadre.SetPointPosition(2, 	AFF_Position.x+SFML_LargeurBoite, 
				AFF_Position.y+SFML_HauteurBoite);
    cadre.SetPointPosition(3, AFF_Position.x, AFF_Position.y+SFML_HauteurBoite);
}



/*
 * ACCESSEURS
 */

// renvois les posotion du coin haut-gauche
Vector2f BoiteInfoAstre::GetPosition() {
    return AFF_Position;
}



