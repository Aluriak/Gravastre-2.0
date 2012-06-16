#include "main.h"

using namespace std;



int main(int argc, char *argv[]) {
    // initialisation de l'aléatoire
    srand(time(NULL));
    QApplication app(argc, argv);

    fenetre w;
    w.show();

    return app.exec();
}




string num2str(float nb) {
    // créer un flux de sortie
    ostringstream oss;
    // écrire un nombre dans le flux
    oss << nb;
    // récupérer une chaîne de caractères
    return oss.str();
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



