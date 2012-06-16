#include "LecteurFichierINI.h"
using namespace std;




/*
 * CONSTRUCTEUR
 */
LecteurFichierINI::LecteurFichierINI(string fic, char separateurs[], unsigned int nbSeparateurs) {
    fichier = fic;
    // allocation dynamique du tableau
    sep = new char[nbSeparateurs];
    for(unsigned int i = 0; i < nbSeparateurs; i++) {
	sep[i] = separateurs[i];
    }
    nbSep = nbSeparateurs;
}

// Surcharge pour les cas où il n'y a qu'un séparateur
LecteurFichierINI::LecteurFichierINI(string fic, char c) {
    fichier = fic;
    // allocation dynamique du tableau
    sep = new char[1];
    sep[0] = c;
    nbSep = 1;
}



/*
 * DESTRUCTEUR
 */
LecteurFichierINI::~LecteurFichierINI() {
    delete sep;
}


/*
 * METHODES PUBLIQUES
 */
// lit le fichier et remplis le vector valeurs. Renvois le nombre de valeurs trouvées, ou -1 en cas d'erreur.
int LecteurFichierINI::lecture() {
    libValeurs(); // on oublis tout ce qu'on a déjà lu
    // ouverture du fichier, renvois du vector vide si erreur.
    ifstream fic(fichier.c_str());
    if(!fic)	return -1;
    
    // initialisations 
    string ligne(""), enr("");
    unsigned int compteLigne = 0; // pour savoir à quelle ligne on est

    // boucle de lecture
    while(getline(fic, ligne)) {
	// on créé un nouveau vector, pour contenir la ligne
	valeurs.push_back(vector<string>(0));
	for(unsigned int i = 0; i < ligne.size(); i++) {
	    // si on rencontre un séparateur
	    if(estSeparateur(ligne[i])) {
		if(enr != "")	valeurs[compteLigne].push_back(enr); // on stocke la chaine enregistrée si elle n'est pas vide
		enr = ""; // on vide la chaine enregistrée
	    }
	    // si ce n'est pas un séparateur
	    else {
		enr += ligne[i];
	    }
	}
	// on est passé à la ligne suivante
	compteLigne++;
    }
    // fin de la lecture de fichier
    return valeurs.size();
}




/*
 * ACCESSEURS
 */
// renvois le vector de résultats
vector< vector<string> >* LecteurFichierINI::getValeurs() {
    return &valeurs;
}




/*
 * METHODES PRIVEES
 */
// renvoi vrai si c fait partie des séparateurs
bool LecteurFichierINI::estSeparateur(char c) {
    for(unsigned int i = 0; i < nbSep; i++) {
	if(c == sep[i])
	    return true;
    }
    return false;
}



// vide le vector de valeur
void LecteurFichierINI::libValeurs() {
    for(unsigned int i = 0; i < valeurs.size(); i++) {
	while(valeurs[i].size() > 0) valeurs[i].pop_back();
    }
    while(valeurs.size() > 0) valeurs.pop_back();

}



