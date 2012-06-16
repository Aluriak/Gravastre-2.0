#include "main.h"

using namespace std;


int main(int argc, char *argv[]) {
    // utilisation des argument de la ligne de commande. Si un 1 est envoyé, on utilise la sortie standard. Sinon, on l'utilise pas.
    // on rappelle que le premier argument est le dossier du lancement 
    bool fichierIni = true; // vrai si on initialise les fichiers au lancement
    bool affichage = false;
    bool exemple = false; // vrai si on doit charger l'exemple, non le fichier
    // on parcours les arguments
    for(int i = 1; i < argc; i++) {
	string str(argv[i]);
	if(str == "-s")	affichage = true;
	if(str == "-i")	fichierIni = true;
	if(str == "-e") exemple = true;
    }

    // préparation des fichiers
    if(fichierIni)	INI_Fichiers();
    
    // lancement du programme
    if(affichage) 
	cout << "\tBienvenue sur " << PRGM_NAME << endl << endl;
    if(affichage) 
	cout << "INI: création d'un RenduGraphique..." << endl;
    RenduGraphique GUI(affichage, exemple);
    GUI.boucleMaitresse();
    return EXIT_SUCCESS;
}




/*
 * FONCTIONS ANNEXES
 */
/*
 * SECONDAIRES
 */
// vide les fichiers de sortie
void INI_Fichiers() {
    ofstream err(FILE_ERR);
    err.close();
}
// renvois nb²
float carre(float nb) {
    return nb*nb;
}



/*
 * CONVERTION DE TYPE
 */
// renvois la chaine équivalent à la valeur
string num2str(int nb) {
    // déclaration du flux
    ostringstream oss;
    // on y fait rentrer le nb
    oss << nb;
    return oss.str();
}

// renvois la chaine équivalent à la valeur   
string float2str(float nb) {
    // déclaration du flux
    ostringstream oss;
    // on y fait rentrer le nb
    oss << nb;
    return oss.str();
}


// renvois la string sous forme de nombre, ou 0
int str2num(string str) {
    int resultat = 0;
    // déclaration du flux
    std::istringstream iss(str);
    // tenter la conversion vers le resultat
    iss >> resultat;
    return resultat;
}

// renvois 0 ou le float contenu dans la chaîne
float str2float(std::string str) {
    float resultat = 0;
    // déclaration du flux
    istringstream iss(str);
    // tenter la conversion vers le resultat
    iss >> resultat;
    return resultat;
}


/*
 * GESTION EXTERNE
 */
// diffuse l'erreur en argument et arrête le programme en FAILURE si arret vrai
void FATAL_ERROR(string error, bool arret) {
    ofstream err(FILE_ERR, ios::app);
    err << error << "\n";
    err.close();
    if(arret)
	exit(EXIT_FAILURE);
}



