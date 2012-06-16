#ifndef LECTUREFICHIERINI_H_INCLUDED
#define LECTUREFICHIERINI_H_INCLUDED


#include <vector>
#include <string>
#include <fstream>




class LecteurFichierINI {
    public:
    //CONSTRUCTEUR
	LecteurFichierINI(std::string, char*, unsigned int);
	LecteurFichierINI(std::string, char);
	~LecteurFichierINI();

    // METHODES
	int lecture(); // lit le fichier et remplis le vector valeurs.

    // ACCESSEURS
	std::vector<std::vector<std::string> >* getValeurs(); // renvois le vector de résultats

    private:
    // ATTRIBUTS
	std::string fichier; // fichier cible
	std::vector< std::vector<std::string> >valeurs; // valeurs retenue du fichier(rangée dans l'ordre d'apparition). Le vector contient, pour chaque ligne du fichier, un vector contenant les différentes valeurs spérarées par sep
	char *sep; // tableau contenant les séparateurs
	unsigned int nbSep; // nombre de séparateur (taille du tableau sep)
	
    // METHODES
	bool estSeparateur(char c); // renvoi vrai si c est un séparateur
	void libValeurs(); // vide le vector de valeur
};





#endif



