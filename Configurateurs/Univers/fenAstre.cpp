#include "main.h"

using namespace std;


/*
 * CONSTRUCTEUR
 */
fenAstre::fenAstre(QWidget *parent, Astre* ast, Astre *mdl) : 
						    QDialog(parent) {
    astre = ast;
    modele = mdl;
    INI_fenAstre(); // création et ordonnance de la fenêtre        
}




/*
 * METHODES
 */



/*
 * SLOTS
 */
void fenAstre::choisirCouleur() {
    *qCouleur = QColorDialog::getColor(*qCouleur, this);
	QPalette palette(*qCouleur);
	a_Couleur->setPalette(palette);
}


void fenAstre::envoiAstre() {
    // on créé la couleur
	Couleur clr;
	    clr.r = qCouleur->red();
	    clr.g = qCouleur->green();
	    clr.b = qCouleur->blue();
    // on recréé l'astre
	    astre->setNom(a_Nom->text().toStdString());
	    astre->setPosition(Vector2f
			(a_PositionX->value(), a_PositionY->value()));
	    astre->setVitesse(Vector2f
			(a_VitesseX->value(), a_VitesseY->value()));
	    astre->setMasseMantisse(a_MasseMantisse->value());
	    astre->setMasseExposant(a_MasseExposant->value());
	    astre->setDiametre(a_Diametre->value());
	    astre->setCouleur(clr);
    // et on ferme !
    accept();
}






/*
 * METHODES PRIVEES
 */
// Retourne un nom au hasard, pioché dans le fichier FICHIER_NOM_ASTRE
string fenAstre::nomAuHasard() {
    ifstream fic(FICHIER_NOM_ASTRE);
    if(!fic) {
	ofstream err("Erreur.txt", ios::app);
	err << "Le fichier FICHIER_NOM_ASTRE n'a pas été ouvert" << endl;
	return "";
    }
    string ligne(""); // contient la ligne courante du fichier
    int inter = 0; // contient les nombres aléatoires
    // chaque ligne est mise dans le vector.
    vector<string> noms(0);
    while(getline(fic, ligne)) {
	noms.push_back(ligne);
    }
    // on sait combien de lignes on a vue, on peut maintenant trouver un nombre aléatoire.
    if(noms.size() == 0)
	return "";
    else {
	// nb aléatoire entre 0 inclus et taille exclu
	inter = rand() % noms.size(); 
	return noms[inter];
    }
}




/*
 * INITIALISATION
 */
void fenAstre::INI_fenAstre() {
    // initialisation du groupe création
	INI_GroupeCreation();
    // initialisation du groupe programme
	INI_GroupePrgm();

    QVBoxLayout *layFen = new QVBoxLayout(this);
	layFen->addWidget(groupCreation);
	layFen->addWidget(groupPrgm);
}



void fenAstre::INI_GroupePrgm() {
    quitter = new QPushButton("Fermer sans enregistrer", this);
    enregistrer = new QPushButton("Enregistrer et fermer", this);
    // connects
	connect(quitter, SIGNAL(clicked()), this, SLOT(reject()));
	connect(enregistrer, SIGNAL(clicked()), this, SLOT(envoiAstre()));
    // layout et groupbox
	QHBoxLayout* layPrgm = new QHBoxLayout;
	    layPrgm->addWidget(quitter);
	    layPrgm->addWidget(enregistrer);
	groupPrgm = new QGroupBox("Validation de l'Astre", this);
	    groupPrgm->setLayout(layPrgm);
}



void fenAstre::INI_GroupeCreation() {
    // nom
	a_LabNom = new QLabel("Nom de l'astre : ", this);
	a_Nom = new QLineEdit(this);
	    if(modele != NULL) {
		a_Nom->setText((modele->getNom()).c_str());
	    } else {
		// si aucun nom n'est prédéfinit, on va chercher dans le fichierFICHIER_NOM_ASTRE, et trouver un nom au hasard
		a_Nom->setText(nomAuHasard().c_str());
	    }
	    a_Nom->setToolTip("Nom de l'astre affichée par le programme. Cette valeur n'a aucune influence sur les calculs de la simulation.");
	    QHBoxLayout* layNom = new QHBoxLayout;
		layNom->addWidget(a_LabNom);
		layNom->addWidget(a_Nom);
    // diamètre
	a_LabDiametre = new QLabel("Diamètre de l'astre : ", this);
	a_Diametre = new QDoubleSpinBox(this);
	    a_Diametre->setMinimum(0.000001);
	    a_Diametre->setMaximum(50);
	    if(modele != NULL) {
		a_Diametre->setValue(modele->getDiametre());
	    }
	    a_Diametre->setToolTip("Diamètre de l'astre en pixel en l'absence de zoom. Cette valeur n'a aucune influence sur les calculs de la simulation.");
	    QHBoxLayout* layDiametre = new QHBoxLayout;
		layDiametre->addWidget(a_LabDiametre);
		layDiametre->addWidget(a_Diametre);
    // Masse
	a_LabMasse = new QLabel("Masse de l'astre : ", this);
	a_LabMasseExposant = new QLabel("Puissance de dix :", this);
	a_MasseMantisse = new QDoubleSpinBox(this);
	    a_MasseMantisse->setRange(-10, 10);
	    a_MasseMantisse->setDecimals(5);
	    a_MasseMantisse->setValue(5.98);
	    a_MasseMantisse->setToolTip("Mantisse du poids de l'astre, en kilogramme");
	a_MasseExposant = new QSpinBox(this);
	    a_MasseExposant->setMinimum(-100);
	    a_MasseExposant->setMaximum(100);
	    a_MasseExposant->setValue(24);
	    a_MasseExposant->setToolTip("Puissance de dix à appliquer au poids de l'astre");
	    if(modele != NULL) {
		a_MasseMantisse->setValue(modele->getMasseMantisse());
		a_MasseExposant->setValue(modele->getMasseExposant());
	    }
	    QHBoxLayout* layMasse = new QHBoxLayout;
		layMasse->addWidget(a_LabMasse);
		layMasse->addWidget(a_MasseMantisse);
		layMasse->addWidget(a_LabMasseExposant);
		layMasse->addWidget(a_MasseExposant);
    // Couleurs
	a_LabCouleur = new QLabel("Couleur de l'astre : ", this);
	a_Couleur = new QPushButton("Choisir la couleur", this);
	    connect(a_Couleur, SIGNAL(clicked()), this, SLOT(choisirCouleur()));
	    a_Couleur->setToolTip("Cliquez pour choisir la couleur de l'astre. Cette valeur n'a aucune influence sur les calculs de la simulation.");
	    QHBoxLayout* layCouleur = new QHBoxLayout;
		layCouleur->addWidget(a_LabCouleur);
		layCouleur->addWidget(a_Couleur);
	    // couleur
	    qCouleur = new QColor(255,255,255);
	    if(modele != NULL) {
		qCouleur->setRed(modele->getCouleur().r);
		qCouleur->setGreen(modele->getCouleur().g);
		qCouleur->setBlue(modele->getCouleur().b);
	    }
    // Positions
	a_LabPositionX = new QLabel("Position en abscisse : ", this);
	a_PositionX = new QDoubleSpinBox(this);
	    a_PositionX->setMinimum(-9e127);
	    a_PositionX->setMaximum(9e127);
	    a_PositionX->setDecimals(5);
	    a_PositionX->setToolTip("Position par rapport à l'origine en ordonnées.\nOn rappelle qu'un pixel en zoom normal équivaut à 500 000 km dans la réalité.\nDe la même façon, on déduit que 300 pixels sont égaux à 1 unité astronomique.");
	a_LabPositionY = new QLabel("Position en ordonnée : ", this);
	a_PositionY = new QDoubleSpinBox(this);
	    a_PositionY->setMinimum(-9e127);
	    a_PositionY->setMaximum(9e127);
	    a_PositionY->setDecimals(5);
	    a_PositionY->setToolTip("Position par rapport à l'origine en abscisse.\nOn rappelle qu'un pixel en zoom normal équivaut à 500 000 km dans la réalité.\nDe la même façon, on déduit que 300 pixels sont égaux à 1 unité astronomique.");
	    QHBoxLayout* layPosition = new QHBoxLayout;
		layPosition->addWidget(a_LabPositionX);
		layPosition->addWidget(a_PositionX);
		layPosition->addWidget(a_LabPositionY);
		layPosition->addWidget(a_PositionY);
	    if(modele != NULL) {
		a_PositionX->setValue(modele->getPosition().x);
		a_PositionY->setValue(modele->getPosition().y);
	    }

    // Vitesses
	a_LabVitesseX = new QLabel("Vitesse en abscisse : ", this);
	a_VitesseX = new QDoubleSpinBox(this);
	    a_VitesseX->setMinimum(-9e127);
	    a_VitesseX->setMaximum(9e127);
	    a_VitesseX->setDecimals(5);
	    a_VitesseX->setToolTip("Vitesse par rapport à l'origine sur l'axe des abscisses.\nOn rappelle que la vitesse correspond au nombre de pixel parcouru par instant t calculé.\nLa vitesse est donc équivalente au nombre de pas de 500 000 km réalisé par instant t.");
	a_LabVitesseY = new QLabel("Vitesse en ordonnée : ", this);
	a_VitesseY = new QDoubleSpinBox(this);
	    a_VitesseY->setMinimum(-9e127);
	    a_VitesseY->setMaximum(9e127);
	    a_VitesseY->setDecimals(5);
	    a_VitesseY->setToolTip("Vitesse par rapport à l'origine sur l'axe des ordonnées.\nOn rappelle que la vitesse correspond au nombre de pixel parcouru par instant t calculé.\nLa vitesse est donc équivalente au nombre de pas de 500 000 km réalisé par instant t.");
	    QHBoxLayout* layVitesse = new QHBoxLayout;
		layVitesse->addWidget(a_LabVitesseX);
		layVitesse->addWidget(a_VitesseX);
		layVitesse->addWidget(a_LabVitesseY);
		layVitesse->addWidget(a_VitesseY);
	    if(modele != NULL) {
		a_VitesseX->setValue(modele->getVitesse().x);
		a_VitesseY->setValue(modele->getVitesse().y);
	    }
/*
    // Accelerations
	a_LabAccelerationX = new QLabel("Acceleration en abscisse : ", this);
	a_AccelerationX = new QDoubleSpinBox(this);
	    a_AccelerationX->setMinimum(-9e127);
	    a_AccelerationX->setMaximum(9e127);
	a_LabAccelerationY = new QLabel("Acceleration en ordonnée : ", this);
	a_AccelerationY = new QDoubleSpinBox(this);
	    a_AccelerationY->setMinimum(-9e127);
	    a_AccelerationY->setMaximum(9e127);
	    QHBoxLayout* layAcceleration = new QHBoxLayout;
		layAcceleration->addWidget(a_LabAccelerationX);
		layAcceleration->addWidget(a_AccelerationX);
		layAcceleration->addWidget(a_LabAccelerationY);
		layAcceleration->addWidget(a_AccelerationY);
	    if(modele != NULL) {
		a_AccelerationX->setValue(modele->getAcceleration().x);
		a_AccelerationY->setValue(modele->getAcceleration().y);
	    } // */

    // Fusion des layouts
	QVBoxLayout *layCreation = new QVBoxLayout(this);
	    layCreation->addLayout(layNom);
	    layCreation->addLayout(layDiametre);
	    layCreation->addLayout(layMasse);
	    layCreation->addLayout(layCouleur);
	    layCreation->addLayout(layPosition);
	    layCreation->addLayout(layVitesse);
	    //layCreation->addLayout(layAcceleration);
	    groupCreation = new QGroupBox("Création de l'Astre", this);
		groupCreation->setLayout(layCreation);
	
	
}




