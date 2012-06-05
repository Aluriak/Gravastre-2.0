#include "main.h"
#include "fenetre.h"

using namespace std;



fenetre::fenetre() {
    setWindowTitle(NOM_PRGM); // titre de la fenetre


    // initialisation des attributs membres
    e_LancerSDL = new QPushButton("Lancer Lecteur", this);
    e_Quit = new QPushButton("Fermer configurateur", this);

    e_ChoixPas = new QSlider(Qt::Horizontal, this);
        e_ChoixPas->setMaximum(600); // maximum du slide
        e_ChoixPas->setMinimum(1); // minimum du slide

        e_AffichePas = new QLabel("Pas = 1");

    e_LabelCurseur = new QLabel("Curseur :", this);
    e_Curseur = new QComboBox(this);
        e_Curseur->addItem("Par défaut");
        e_Curseur->addItem("Viseur");


    e_LabelConfig = new QLabel("Taille du lecteur", this);
    e_ChoixConfig = new QComboBox(this);
        e_ChoixConfig->addItem("600 * 400");
        e_ChoixConfig->addItem("600 * 600");
        e_ChoixConfig->addItem("1000 * 600");
        e_ChoixConfig->addItem("1200 * 720");

    e_ECH = new QPushButton("Echelle présente", this);
        ECH_presente = true;

    QVBoxLayout *LayPrinc = new QVBoxLayout; // fenetre
        LayPrinc->addWidget(e_ECH);
        LayPrinc->addWidget(e_AffichePas);
        LayPrinc->addWidget(e_ChoixPas);
        LayPrinc->addWidget(e_LabelCurseur);
        LayPrinc->addWidget(e_Curseur);
        LayPrinc->addWidget(e_LabelConfig);
        LayPrinc->addWidget(e_ChoixConfig);
        LayPrinc->addWidget(e_LancerSDL);
        LayPrinc->addWidget(e_Quit);
    setLayout(LayPrinc);

    // connects
    QObject::connect(e_ECH, SIGNAL(clicked()), this, SLOT(ChangementECH()));
    QObject::connect(e_ChoixPas, SIGNAL(valueChanged(int)), this, SLOT(ChangementPas(int)));
    QObject::connect(e_LancerSDL, SIGNAL(clicked()), this, SLOT(Lancer_PRGM()));
    QObject::connect(e_Quit, SIGNAL(clicked()), qApp, SLOT(quit()));
}


// dans le fichier FILE_PROP, on écrit les propriétés en fonction des choix fait par l'utilisateur
void fenetre::EcritureFichier()
{
    ofstream fil(FILE_PROP);

    int pas = e_ChoixPas->value();
    QString bin = e_ChoixConfig->currentText();
    string config = bin.toStdString();
    if(config == "600 * 400")
    {
        fil << ":600" <<endl;
        fil << ":400" <<endl;
    }
    else if(config == "600 * 600")
    {
        fil << ":600" <<endl;
        fil << ":600" <<endl;
    }
    else if(config == "1000 * 600")
    {
        fil << ":1000" <<endl;
        fil << ":600" <<endl;
    }
    else if(config == "1200 * 720")
    {
        fil << ":1200" <<endl;
        fil << ":720" <<endl;
    }

    fil << ":" << pas << endl;
    if(ECH_presente) // si le bouton échelle est enfoncé
        fil << ":1" << endl;
    else
        fil << ":0" << endl;

    // curseur
    bin = e_Curseur->currentText();
    config = bin.toStdString();
    if(config == "Par défaut")
        fil << ":0" << endl;
    else
        fil << ":1" << endl;


    // on ferme fichier
    fil.close();
}


void fenetre::ChangementECH()
{
    if(ECH_presente)
    {
        ECH_presente = false;
        e_ECH->setText("Pas d'échelle");
    }
    else
    {
        ECH_presente = true;
        e_ECH->setText("Echelle présente");
    }
}


void fenetre::ChangementPas(int nvPas)
{
    QString str = "Pas de ";
    string bin = num2str(nvPas);
    for(unsigned int i = 0; i < bin.size(); i++)
        str += bin[i];
    e_AffichePas->setText(str);
}

void fenetre::Lancer_PRGM()
{
    EcritureFichier();
    QProcess::startDetached (CHEMIN_SDL);
}


