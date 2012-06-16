#include "../main.h"

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
    // Données à afficher (on initialise ce qui est constant)
	// le nom est mit en plus gros
	donnees[0].SetSize(donnees[0].GetSize()*3);
	donnees[1] = String("", *police, taillePolice);
	donnees[2] = String("", *police, taillePolice);
	donnees[3] = String("", *police, taillePolice);
	donnees[4] = String("", *police, taillePolice);
	donnees[5] = String("", *police, taillePolice);
	donnees[6] = String("", *police, taillePolice);
	donnees[7] = String("", *police, taillePolice);
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
    // Quelques détails
	cadre.EnableFill(false); // Il n'est pas plein !
	cadre.SetOutlineWidth(1); // large de 10 pixels
    // gestion de la variation
	for(int i = 0; i < 6; i++) {
	    tabEvolValeurs[i] = 0;
	    tabVariation[i] = true; // par défaut, on croît
	}
	// images
	problemeImage = false; // a priori, pas de problème
	if( !varCroit.LoadFromFile(BIA_IMAGE_CROIT) || 
	    !varDecroit.LoadFromFile(BIA_IMAGE_DECROIT)) {
	    FATAL_ERROR("BIA: Les images de variations n'ont pas été ouvertes", false);
	    problemeImage = true; // on signal qu'il ya eu un problème
	} else {
	    // si pas d'erreur, on créer une transparence sur la couleur noire
	    varCroit.CreateMaskFromColor(Color(0,0,0));
	    varDecroit.CreateMaskFromColor(Color(0,0,0));
	}
}





/*
 * METHODES PUBLIQUES
 */
// affiche tous les objets drawables sur app
void BoiteInfoAstre::Draw(Astre *astre) {
    float inter = 0; // contient la valeur étudiée de l'astre à traiter (permet de limiter le nombre d'accès à cette valeur via l'astre)
    float calcul = 0; // contient des valeurs de calculs (évite de les refaire)
    // CADRE
    // on affiche le cadre
	app->Draw(cadre);

    // DONNEES
    // on initialise les sf::String 
    donnees[0] = astre->GetNom();
	// le nom est mit en plus gros
	donnees[0].SetSize(donnees[0].GetSize()*3);
	donnees[1].SetText("Masse : " + 
		float2str(astre->GetMasse()/CONVERTION_KG));
	inter = astre->GetPosition().x/CONVERTION_DIST;
	    donnees[2].SetText("PositionX : " + float2str(inter));
	    calcul = inter - tabEvolValeurs[0];
	    if(calcul < 0.0)
		tabVariation[0] = false;
	    else if(calcul > 0.0)
		tabVariation[0] = true;
	    tabEvolValeurs[0] = inter;
	inter = astre->GetPosition().y/CONVERTION_DIST;
	    donnees[3].SetText("PositionY : " + float2str(inter));
	    calcul = inter - tabEvolValeurs[1];
	    if(calcul < 0.0)
		tabVariation[1] = false;
	    else if(calcul > 0.0)
		tabVariation[1] = true;
	    tabEvolValeurs[1] = inter;
	inter = astre->GetVitesse().x/CONVERTION_VIT;
	    donnees[4].SetText("VitesseX : " + float2str(inter));
	    calcul = inter - tabEvolValeurs[2];
	    if(calcul < 0.0)
		tabVariation[2] = false;
	    else if(calcul > 0.0)
		tabVariation[2] = true;
	    tabEvolValeurs[2] = inter;
	inter = astre->GetVitesse().y/CONVERTION_VIT;
	    donnees[5].SetText("VitesseY : " + float2str(inter));
	    calcul = inter - tabEvolValeurs[3];
	    if(calcul < 0.0)
		tabVariation[3] = false;
	    else if(calcul > 0.0)
		tabVariation[3] = true;
	    tabEvolValeurs[3] = inter;
	inter = astre->GetAcceleration().x/CONVERTION_VIT;
	    donnees[6].SetText("AccelerationX : " + float2str(inter));
	    calcul = inter - tabEvolValeurs[4];
	    if(calcul < 0.0)
		tabVariation[4] = false;
	    else if(calcul > 0.0)
		tabVariation[4] = true;
	    tabEvolValeurs[4] = inter;
	inter = astre->GetAcceleration().y/CONVERTION_VIT;
	    donnees[7].SetText("AccelerationY : " + float2str(inter));
	    calcul = inter - tabEvolValeurs[5];
	    if(calcul < 0.0)
		tabVariation[5] = false;
	    else if(calcul > 0.0)
		tabVariation[5] = true;
	    tabEvolValeurs[5] = inter;
    
    // on place les différents éléments 
    float coordX = 0, coordY = 0;
    for(int i = 0; i < 8; i++) {
	// coordonnées de base
	coordX = AFF_Position.x + 20;
	coordY = AFF_Position.y + 30 + i*2*taillePolice;
	// corrections pour le titre
	if(i == 0) {
	    coordX += 10; // le nom est centré
	    coordY -= 20; // le nom est relevé
	}
	// application
	donnees[i].SetPosition(coordX, coordY);
	app->Draw(donnees[i]);
	// affichage des flèches de variation si traitement de pos, vit ou acc.
	if(!problemeImage && i > 1) {
	    Sprite sprite;
	    if(tabVariation[i-2]) // si croissance
		sprite.SetImage(varCroit);
	    else // si décroissance
		sprite.SetImage(varDecroit);
	    sprite.SetPosition(	AFF_Position.x+10, 
				    AFF_Position.y+30+i*2*taillePolice);
	    app->Draw(sprite);
	}
    } // fin boucle d'affichage
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



