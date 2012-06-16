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



