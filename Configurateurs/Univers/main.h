#ifndef MAIN_H_INCLUDED // portection header contre inclusion multiple.
#define MAIN_H_INCLUDED


/*
 * BIBLIOTHEQUES
 */
// C++
#include <iostream> // I/O
#include <fstream> // fichier
#include <sstream> // flux
#include <string> // chaîne
// Qt
#include <QApplication> // Qt application
#include <QObject> // objets
#include <QtGui> // GUI
#include <QProcess> // lancement de programmes externes





/*
 * DECLARATIONS GLOBALES
 */
// fonctions annexes
    std::string num2str(float);
// énumération permettant de définir la façon dont est déterminée la position du satellite en fonction de l'astre
enum MODE_PLACEMENT {Position, Vitesse};
// Structures de simplification d'écriture
struct Vector2f {
    Vector2f() {x=y=0;};
    Vector2f(float X, float Y) {x=X; y=Y;};
    std::string affichage() {std::string inter = 
	"(" + num2str(x) + ";" + num2str(y) + ")";
	return inter;};
    float x;
    float y;
};
struct Couleur {
    Couleur() {r=g=b=255;};
    Couleur(int d, int e, int f) {r=d;g=e;b=f;};
    std::string affichage() {std::string inter = 
	"(" + num2str(r) + ";" + num2str(g) + ";" + num2str(b) + ")";
	return inter;};
    int r;
    int g;
    int b;
};




/*
 * DEFINES
 */
// Général
    #define PRGM_NOM "Configurateur de l'Univers de Gravastre"
    #define FICHIER_CIBLE "./Fichiers/AST.Alk"
    #define FICHIER_NOM_ASTRE "./NomsAstres"
    #define CHEMIN_GRAVASTRE "./Gravastre"
    #define PRGM_LARGEUR 667
    #define PRGM_HAUTEUR 500
// Valeurs
    #define TABLE_NB_COLONNE_ASTRE 6




/*
 * HEADERS
 */
#include "Astre.h"
#include "fenAstre.h"
#include "fenetre.h"


#endif // MAIN_H_INCLUDED
