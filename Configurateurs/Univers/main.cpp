#include "main.h"

using namespace std;



int main(int argc, char *argv[]) {
    // initialisation de l'al�atoire
    srand(time(NULL));
    QApplication app(argc, argv);

    fenetre w;
    w.show();

    return app.exec();
}




string num2str(float nb) {
    // cr�er un flux de sortie
    ostringstream oss;
    // �crire un nombre dans le flux
    oss << nb;
    // r�cup�rer une cha�ne de caract�res
    return oss.str();
}




/*
 * GESTION EXTERNE
 */
// diffuse l'erreur en argument et arr�te le programme en FAILURE si arret vrai
void FATAL_ERROR(string error, bool arret) {
    ofstream err(FILE_ERR, ios::app);
    err << error << "\n";
    err.close();
    if(arret)
	exit(EXIT_FAILURE);
}



