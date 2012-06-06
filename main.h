#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


/*
 * BIBLIOTHEQUES
 */
// C++
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <cmath>
    #include <vector>
// SFML	
    #include <SFML/System.hpp>
    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>



/*
 * DEFINES
 */
// PRGM
    #define PRGM_NAME "Gravastre 2.0"
    #define PRGM_TAILLEBOITEINFO_Y 250
    #define PRGM_TAILLEBOITEINFO_X 150
    #define PRGM_QUESTION_QUITTER "Quitter ?"
    #define PRGM_NOM_REFERENCE "Origine"
// FILES
    #define FILE_ERR "Fichiers/Erreur.txt" // contient les messages d'erreur
    #define FILE_INI "Fichiers/INI.Alk"    // contient les valeurs utilisateurs
    #define FILE_AST "Fichiers/AST.Alk"    // contient les astres
    #define FILE_OUT "Fichiers/Out.Alk"	// deviens sortie standard si besoin



/*
 * DECLARATIONS
 */
// énumérations
    // Utilisé pour savoir où l'utilisateur à cliqué dans l'interface
    // 	- en dehors de l'interface
    // 	- boite d'information sur l'astre sélectionné
    // 	- barre d'interface du haut
    enum OBJET_INTERFACE {Dehors, BoiteInfo, Barre};




/*
 * HEADERS
 */
class Univers; 
class Astre; 

#include "Interface/BoiteInfoAstre.h"
#include "Interface/BoiteHauteInterface.h"
#include "LecteurFichierINI.h"
#include "Astre.h"
#include "ReferenceUnivers.h"
#include "Univers.h"
#include "RenduGraphique.h"


/*
 * FONCTIONS
 */
// ANNEXES (main.cpp)
    void INI_Fichiers(); // vide les fichiers de sortie
    float carre(float nb); // renvois nb²
    std::string num2str(int); // renvois la chaine équivalent à la valeur
    std::string float2str(float); // renvois la chaine équivalent à la valeur
    int str2num(std::string); // renvois la string sous forme de nombre, ou 0
    float str2float(std::string str); // renvois 0 ou le float contenu dans la chaîne
    void FATAL_ERROR(std::string error, bool arret = true); // diffuse l'erreur en arguement et arrête le programme en FAILURE


#endif



