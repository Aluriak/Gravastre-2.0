#include "main.h"

using namespace std;



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    fenetre w;
    w.show();

    return app.exec();
}




string num2str(int nb) {
    // cr�er un flux de sortie
    ostringstream oss;
    // �crire un nombre dans le flux
    oss << nb;
    // r�cup�rer une cha�ne de caract�res
    return oss.str();
}



