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
#include <QtGui> // GUI
#include <QProcess> // lancement de programmes externes


/*
 * HEADERS
 */
#include "fenetre.h"



/*
 * DEFINES
 */
// Général
    #define NOM_PRGM "Configurateur des graphismes de Gravastre"
    #define FICHIER_CIBLE "Fichiers/INI.Alk"
    #define GRAVASTRE_COULEUR_INTERFACE 80,0,255
    #define GRAVASTRE_CHEMIN_POLICE "Fichiers/Ressources/NEUROPOLX.ttf"
    #define GRAVASTRE_CHEMIN_MUSIQUE "Fichiers/Ressources/WelcomeToNova-Emitremmus.flac"
// Valeurs
    #define RESOLUTION_PERSONNALISEE "Personnalisé"
    #define RESOLUTION_800_600 "800*600"
    #define RESOLUTION_1024_600 "1024*600"
    #define RESOLUTION_1200_700 "1200*700"
    #define RESOLUTION_1400_700 "1400*700"
    #define DEFINITION_8 "8 bits"
    #define DEFINITION_16 "16 bits"
    #define DEFINITION_32 "32 bits"


#endif // MAIN_H_INCLUDED
