#include "main.h"
#include "fenetre.h"

using namespace std;



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenetre w;
    w.show();

    return app.exec();
}



// renvois le nombre écrit en format ascii-chaîne, ou 0 si c'est pas un nombre
int str2num(string str)
{
    int var = 0;

    // boucle de vérification
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i] < 48 || str[i] > 57)
            return 0; // si c'est pas un nombre en ASCII, on retourne 0.
    }

    // deuxième boucle : cette fois-ci on lit vraiment le nombre
    for(unsigned int i = 0; i < str.size(); i++)
    {
        var += (str[i] - 48) * pow(10, str.size() - (i+1)); // on ajoute progressivement à var les bonne valeurs.
    }
    return var;
}


int pow(int a, int b)
{
    int result = 1;
    while(b)
    {
        result *= a;
        b--;
    }
    return result;
}

string num2str(int nb)
{
    // créer un flux de sortie
    ostringstream oss;
    // écrire un nombre dans le flux
    oss << nb;
    // récupérer une chaîne de caractères
    return oss.str();
}

