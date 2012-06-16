#include "main.h"

using namespace std;



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    fenetre w;
    w.show();

    return app.exec();
}




string num2str(int nb) {
    // créer un flux de sortie
    ostringstream oss;
    // écrire un nombre dans le flux
    oss << nb;
    // récupérer une chaîne de caractères
    return oss.str();
}



