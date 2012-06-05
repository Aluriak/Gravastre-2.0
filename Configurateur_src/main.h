#ifndef MAIN_H_INCLUDED // portection header contre inclusion multiple.
#define MAIN_H_INCLUDED



//==========================
// inclusions de bib
//==========================

// BIBLIOTHEQUES
#include <iostream>
#include <fstream> // fichier
#include <string>
#include <sstream> // bib spéciale : http://cpp.developpez.com/faq/cpp/?page=strings#STRINGS_numtostr
// Qt
#include <QApplication> // Qt application
#include <QSlider>
#include <QLabel> // texte
#include <QPushButton>
#include <QVBoxLayout> // layouts de placement de widget
#include <QHBoxLayout>
#include <QWidget> // widgets
#include <QDialog>
#include <QComboBox> // faire des combobox
#include <QProcess> // démarrage de programme annexes
//#include <QProcessEnvironment> // création et gestion de processus fils
// headers
#include "fenetre.h"


// PROTOTYPES
int pow(int a, int b);
int str2num(std::string str);
std::string num2str(int nb);


// DEFINES
#define FILE_PROP "Definitions/Proprietes.Alk"
#define NOM_PRGM "Configurateur du Lecteur Aluriak"
#define CHEMIN_SDL "./Cartographe_LNA"



#endif // MAIN_H_INCLUDED
