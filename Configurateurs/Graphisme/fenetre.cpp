#include "main.h"

using namespace std;




/*
 * CONSTRUCTEUR
 */
fenetre::fenetre(QWidget *parent) : QWidget(parent) {
    initialisationFenetre();
}




/*
 * SLOTS
 */
// écriture dans le fichier cible suivant les états et paramètres choisis par l'utilisateur
void fenetre::ecritureFichier() {
    // ouverture du fichier cible, + gestion erreur
    ofstream fic(FICHIER_CIBLE); // on supprime ce qu'il y avait avant
    if(!fic) {
	ofstream err("Erreur.txt", ios::app);
	err << "Erreur : le fichier cible n'a pas pu être ouvert" << endl;
	err.close();
	return;
    }
    // Variables intermédiaires
    bool inverserZoom = false;
    bool inverserDefil = false;
    bool activerMusique = false;
    if(OPT_InvZoom->isChecked()) inverserZoom = true;
    if(OPT_InvDefil->isChecked()) inverserDefil = true;
    if(OPT_ActiverMusique->isChecked()) activerMusique = true;
    int definition = 32;
    if(ECRAN_Definition->currentText() == DEFINITION_16)
	definition = 16;
    if(ECRAN_Definition->currentText() == DEFINITION_8)
	definition = 8;
    int affichageTrajectoire = 1;
    if(AFF_Trajectoire->currentText() == "Tous les astres")
	affichageTrajectoire = 2;
    else if(AFF_Trajectoire->currentText() == "Aucune trajectoire")
	affichageTrajectoire = 0;

    // on va écrire entièrement le fichier
    // PREMIERE LIGNE
	// Taille de l'écran
	    fic << ECRAN_TailleEcranX->value()
	    <<":"<< ECRAN_TailleEcranY->value()
	// Taille de le boite d'info astre (constante)
	    <<":"<< 250
	    <<":"<< 250
	// Hauteur de la barre haute d'interface (constante)
	    <<":"<< 20
	// Définition
	    <<":"<< definition
	// Couleur de l'interface
	    <<":"<< qCouleurInterface->red()
	    <<":"<< qCouleurInterface->green()
	    <<":"<< qCouleurInterface->blue()
	// Anticrénelage
	    <<":"<< ECRAN_Anticrenelage->value()
	// FPS
	    <<":"<< ECRAN_FPS->value()
	// Tolérance de sélection
	    <<":"<< AFF_BordSelection->value()
	// Taille de la police pour les noms des astres
	    <<":"<< AFF_TaillePolice->value()
	// Taille de la police pour la boite d'info astre (constante)
	    <<":"<< 14
	// Taille de la police ppr l'interface ahute (constante)
	    <<":"<< 12
    // DEUXIEME LIGNE
	<<":"<<endl<< qCheminPolice->toStdString()
    // TROISIEME LIGNE
	<<":"<<endl<< VAL_PasFleche->value()
	    <<":"<< VAL_PrecisionZoom->value()
	    <<":"<< VAL_PasTabulation->value()
	    <<":"<< VAL_PrecisionClic->value()
	    <<":"<< affichageTrajectoire
	    <<":"<< AFF_BordTrajectoire->value()
	    <<":"<< inverserZoom
	    <<":"<< inverserDefil
	    <<":"<< activerMusique
    // QUATRIEME LIGNE
	<<":"<<endl<< qCheminMusique->toStdString() << ":";


    // le fichier à été totalement écrit. On le ferme
    fic.close();
}




void fenetre::changementResolution() {
    string resolution = ECRAN_TailleEcran->currentText().toStdString();
    if(resolution == RESOLUTION_PERSONNALISEE) {
	ECRAN_TailleEcranX->setEnabled(true);
	ECRAN_TailleEcranY->setEnabled(true);
    } else {
	ECRAN_TailleEcranX->setEnabled(false);
	ECRAN_TailleEcranY->setEnabled(false);
	if(resolution == RESOLUTION_1400_700) {
	    ECRAN_TailleEcranX->setValue(1400);
	    ECRAN_TailleEcranY->setValue(700);
	} else if(resolution == RESOLUTION_1200_700) {
	    ECRAN_TailleEcranX->setValue(1200);
	    ECRAN_TailleEcranY->setValue(700);
	} else if(resolution == RESOLUTION_1024_600) {
	    ECRAN_TailleEcranX->setValue(1024);
	    ECRAN_TailleEcranY->setValue(600);
	} else if(resolution == RESOLUTION_800_600) {
	    ECRAN_TailleEcranX->setValue(800);
	    ECRAN_TailleEcranY->setValue(600);
	}
    }
}





// Demande une couleur à l'utilisateur, puis utilise cette couleur comme couleur d'interface
void fenetre::choisirCouleurInterface() {
    *qCouleurInterface = QColorDialog::getColor(*qCouleurInterface, this);
	QPalette palette(*qCouleurInterface);
	AFF_CouleurInterface->setPalette(palette);
}


// demande un chemin vers une police de l'utilisateur, et la sauvegarde dans le QString qCheminPolice
void fenetre::choisirCheminPolice() {
    QString inter = QFileDialog::getOpenFileName(this, "Choisir la police", QString(), "Polices (*.ttf)");
    if(inter == *qCheminPolice) {
	//QMessageBox::information(this, "Police", "Vous avez conservé la police choisie utlérieurement");
    } else {
	//QMessageBox::information(this, "Police", "Vous avez choisi la police " + inter);
	*qCheminPolice = inter;
	AFF_LabPolice->setText(inter);
    }
}


// demande un chemin vers une musique de format lisible par la SFML, et le sauvegarde dans la QString qCheminMusique
void fenetre::choisirCheminMusique() {
    QString inter = QFileDialog::getOpenFileName(this, "Choisir la musique", QString(), "Musiques (*.flac *.wav *.ogg)");
    if(inter == *qCheminMusique) {
	//QMessageBox::information(this, "Musique", "Vous avez conservé la musique choisie utlérieurement");
    } else {
	//QMessageBox::information(this, "Musique", "Vous avez choisi la musique " + inter);
	*qCheminMusique = inter;
	VAL_LabMusique->setText(inter);
    }
}


// enregistre la configuration, puis quitte le programme
void fenetre::EnregistrerEtQuitter() {
    ecritureFichier(); // on écrit dans le fichier
    qApp->quit(); // on quitte
}







/*
 * METHODE PRIVEE
 */
void fenetre::initialisationFenetre() {
    // Groupe Graphique
    groupGraphique = new QGroupBox("Graphisme", this);
	groupGraphique->setLayout(INI_GroupeGraphique());

    // Groupe Navigation
    groupNavigation = new QGroupBox("Navigation", this);
	groupNavigation->setLayout(INI_GroupeNavigation());
    

    // Groupe programme
    groupPrgm = new QGroupBox("Programme", this);
	groupPrgm->setLayout(INI_GroupeProgramme());


    // fenetre
    QVBoxLayout* layFen = new QVBoxLayout(this);
	layFen->addWidget(groupGraphique);
	layFen->addWidget(groupNavigation);
	layFen->addWidget(groupPrgm); // contenant les boutons
}









// Renvois le layout contenant tous les widgets initialisés et placés du groupbox écran et affichage
QVBoxLayout* fenetre::INI_GroupeGraphique() {
    // ECRAN
    groupEcran = new QGroupBox("Ecran", this);
	QVBoxLayout* layEcran = new QVBoxLayout;
	// 1: TAILLE ECRAN
	// widget
	    ECRAN_LabTailleEcran = new QLabel("Taille de l'écran", this);
	    ECRAN_TailleEcran = new QComboBox(this);
		ECRAN_TailleEcran->addItem(RESOLUTION_1400_700);
		ECRAN_TailleEcran->addItem(RESOLUTION_1200_700);
		ECRAN_TailleEcran->addItem(RESOLUTION_1024_600);
		ECRAN_TailleEcran->addItem(RESOLUTION_800_600);
		ECRAN_TailleEcran->addItem(RESOLUTION_PERSONNALISEE);
		ECRAN_TailleEcran->setToolTip("Taille en pixel de l'écran");
	    ECRAN_TailleEcranX = new QSpinBox(this);
		ECRAN_TailleEcranX->setMaximum(10000);
		ECRAN_TailleEcranX->setMinimum(100);
		ECRAN_TailleEcranX->setValue(1400);
		ECRAN_TailleEcranX->setToolTip("Taille en x");
	    ECRAN_TailleEcranY = new QSpinBox(this);
		ECRAN_TailleEcranY->setMaximum(10000);
		ECRAN_TailleEcranY->setMinimum(100);
		ECRAN_TailleEcranY->setValue(700);
		ECRAN_TailleEcranY->setToolTip("Taille en y");
		// on les rends non-modifiables
		ECRAN_TailleEcranX->setEnabled(false);
		ECRAN_TailleEcranY->setEnabled(false);
	// Layouts
	    QHBoxLayout* layTailleEcran = new QHBoxLayout;
		layTailleEcran->addWidget(ECRAN_LabTailleEcran);
		layTailleEcran->addWidget(ECRAN_TailleEcran);
		layTailleEcran->addWidget(ECRAN_TailleEcranX);
		layTailleEcran->addWidget(ECRAN_TailleEcranY);
	// connect
	    connect(ECRAN_TailleEcran, SIGNAL(currentIndexChanged(QString)),
		    this, SLOT(changementResolution()));
	// 2: DEFINITION COULEURS
	// widget
	    ECRAN_LabDefinition = new QLabel("Définition des couleurs", this);
	    ECRAN_Definition = new QComboBox(this);
		ECRAN_Definition->addItem(DEFINITION_32);
		ECRAN_Definition->addItem(DEFINITION_16);
		ECRAN_Definition->addItem(DEFINITION_8);
		ECRAN_Definition->setToolTip("Nombres de bits utilisés pour coder une couleur. A moins d'avoir de sérieux problèmes de carte graphique ou un besoin d'économiser les nanosecondes, le mode 32 bits est tout indiqué");
	// Layouts
	    QHBoxLayout* layDefinition = new QHBoxLayout;
		layDefinition->addWidget(ECRAN_LabDefinition);
		layDefinition->addWidget(ECRAN_Definition);
	// 3: FPS
	// widget
	    ECRAN_LabFPS = new QLabel("Images par secondes", this);
	    ECRAN_FPS = new QSpinBox(this);
		ECRAN_FPS->setMaximum(100);
		ECRAN_FPS->setMinimum(0.01);
		ECRAN_FPS->setValue(30);
		ECRAN_FPS->setToolTip("Nombre d'image par secondes affichées, et accessoirement le nombre de dt calculés dans le même temps. 30 c'est bien, 60 c'est très rapide");
	// Layouts
	    QHBoxLayout* layFPS = new QHBoxLayout;
		layFPS->addWidget(ECRAN_LabFPS);
		layFPS->addWidget(ECRAN_FPS);
	// 4: ANTICRENELAGE
	// widget
	    ECRAN_LabAnticrenelage = new QLabel("Niveaux d'anticrénelage", 
						this);
	    ECRAN_Anticrenelage = new QSpinBox(this);
		ECRAN_Anticrenelage->setMaximum(32);
		ECRAN_Anticrenelage->setMinimum(0);
		ECRAN_Anticrenelage->setValue(4);
		ECRAN_Anticrenelage->setToolTip("Nombre de lissage par affichage. N'allez pas au-dessus de 4 si vous ne savez pas pourquoi vous le faites");
	// Layouts
	    QHBoxLayout* layAnticrenelage = new QHBoxLayout;
		layAnticrenelage->addWidget(ECRAN_LabAnticrenelage);
		layAnticrenelage->addWidget(ECRAN_Anticrenelage);
    // layouts d'écran
    layEcran->addLayout(layTailleEcran);
    layEcran->addLayout(layDefinition);
    layEcran->addLayout(layFPS);
    layEcran->addLayout(layAnticrenelage);
    groupEcran->setLayout(layEcran);

    // AFFICHAGE
    groupAffichage = new QGroupBox("Affichage", this);
	QVBoxLayout* layAffichage = new QVBoxLayout;
	// 1: SIGNALISATION DE LA SELECTION
	// widget
	    AFF_LabBordSelection = new QLabel("Signalisation de la sélection",
						this);
	    AFF_BordSelection = new QSpinBox(this);
		AFF_BordSelection->setMaximum(10);
		AFF_BordSelection->setMinimum(1);
		AFF_BordSelection->setValue(5);
		AFF_BordSelection->setToolTip("Augmentation de l'astre sélectionné en pixel, de façon purement visuelle. 2 pixels, c'est peu, 6 pixels, c'est beaucoup");
	// Layouts
	    QHBoxLayout* layBordSelection = new QHBoxLayout;
		layBordSelection->addWidget(AFF_LabBordSelection);
		layBordSelection->addWidget(AFF_BordSelection);
	// 2: TRAJECTOIRES
	// widget
	    AFF_LabTrajectoire = new QLabel("Affichage des trajectoires", this);
	    AFF_Trajectoire = new QComboBox(this);
		AFF_Trajectoire->addItem("Tous les astres");
		AFF_Trajectoire->addItem("Seulement la sélection");
		AFF_Trajectoire->addItem("Aucune trajectoire");
		AFF_Trajectoire->setToolTip("Choisissez quels astres verront leur trajectoire affichée. Les calculs menés pour les affichages de trajectoires sont lourds, évitez donc d'afficher toutes les trajectoire quand 20 astres se promènent dans votre Univers.");
	    AFF_BordTrajectoire = new QDoubleSpinBox(this);
		AFF_BordSelection->setRange(0.001,100);
		AFF_BordSelection->setValue(0.3);
		AFF_BordSelection->setToolTip("Taille des points représentant la trajectoire");
	// Layouts
	    QHBoxLayout* layTrajectoire = new QHBoxLayout;
		layTrajectoire->addWidget(AFF_LabTrajectoire);
		layTrajectoire->addWidget(AFF_Trajectoire);
		layTrajectoire->addWidget(AFF_BordTrajectoire);
	// 3: TAILLE POLICE ASTRE NOM
	// widget
	    AFF_LabTaillePolice = new QLabel("Taille de la police des astres", 
					    this);
	    AFF_TaillePolice = new QSpinBox(this);
		AFF_TaillePolice->setMaximum(42);
		AFF_TaillePolice->setMinimum(1);
		AFF_TaillePolice->setValue(12);
		AFF_TaillePolice->setToolTip("Taille de la police pour les noms des astres affichés dans l'Univers. La taille 12 est idéale");
	// Layouts
	    QHBoxLayout* layTaillePolice = new QHBoxLayout;
		layTaillePolice->addWidget(AFF_LabTaillePolice);
		layTaillePolice->addWidget(AFF_TaillePolice);
	// 4: POLICE UTILISEE
	// widget
	    AFF_LabPolice = new QLabel(GRAVASTRE_CHEMIN_POLICE, this);
	    AFF_Police = new QPushButton("Autre Police", this);
		AFF_Police->setToolTip("Police utilisée dans le programme");
		    qCheminPolice = new QString(GRAVASTRE_CHEMIN_POLICE);
	// Layouts
	    QHBoxLayout* layPolice = new QHBoxLayout;
		layPolice->addWidget(AFF_LabPolice);
		layPolice->addWidget(AFF_Police);
	// connect
	    connect(AFF_Police, SIGNAL(clicked()),
		    this, SLOT(choisirCheminPolice()));
	// 5: COULEUR INTERFACE
	// widget
	    AFF_LabCouleurInterface = new QLabel("Couleur de l'interface", 
						    this);
	    AFF_CouleurInterface = new QPushButton("Changer couleur", this);
		AFF_CouleurInterface->setToolTip("Couleur dominante de l'interface du programme");
		    qCouleurInterface = new QColor(GRAVASTRE_COULEUR_INTERFACE);
		QPalette palette(*qCouleurInterface); // couleur du bouton
		AFF_CouleurInterface->setPalette(palette);
	// Layouts
	    QHBoxLayout* layCouleurInterface = new QHBoxLayout;
		layCouleurInterface->addWidget(AFF_LabCouleurInterface);
		layCouleurInterface->addWidget(AFF_CouleurInterface);
	// connect
	    connect(AFF_CouleurInterface, SIGNAL(clicked()),
		    this, SLOT(choisirCouleurInterface()));
    // layout d'affichage
    layAffichage->addLayout(layBordSelection);
    layAffichage->addLayout(layTrajectoire);
    layAffichage->addLayout(layTaillePolice);
    layAffichage->addLayout(layPolice);
    layAffichage->addLayout(layCouleurInterface);
    groupAffichage->setLayout(layAffichage);

    // LAYOUT GRAPHIQUE
    QVBoxLayout* layGraphique = new QVBoxLayout;
	layGraphique->addWidget(groupEcran);
	layGraphique->addWidget(groupAffichage);
    return layGraphique;
}








// Renvois le layout contenant tous les widgets initialisés et placés du groupbox Valeurs et Options
QVBoxLayout* fenetre::INI_GroupeNavigation() {
    groupValeurs = new QGroupBox("Valeurs", this);
    QVBoxLayout *layValeurs = new QVBoxLayout;
	// 1: PAS FLECHE
	// widget
	    VAL_LabPasFleche = new QLabel("Pas flèche", this);
	    VAL_PasFleche = new QSpinBox(this);
		VAL_PasFleche->setMaximum(1000);
		VAL_PasFleche->setMinimum(0);
		VAL_PasFleche->setValue(20);
		VAL_PasFleche->setToolTip("Déplacement en pixel lors de la pression surla flèches du clavier");
	// Layouts
	    QHBoxLayout* layPasFleche = new QHBoxLayout;
		layPasFleche->addWidget(VAL_LabPasFleche);
		layPasFleche->addWidget(VAL_PasFleche);
	// 2: PAS TABULATION
	// widget
	    VAL_LabPasTabulation = new QLabel("Pas tabulation", this);
	    VAL_PasTabulation = new QSpinBox(this);
		VAL_PasTabulation->setMaximum(10000);
		VAL_PasTabulation->setMinimum(0);
		VAL_PasTabulation->setValue(100);
		VAL_PasTabulation->setToolTip("Nombre d'instants t calculés lors de la pression sur la touche tabulation lorsque le tampon est vide ou incorrect");
	// Layouts
	    QHBoxLayout* layPasTabulation = new QHBoxLayout;
		layPasTabulation->addWidget(VAL_LabPasTabulation);
		layPasTabulation->addWidget(VAL_PasTabulation);
	// 3: PRECISION CLIC
	// widget
	    VAL_LabPrecisionClic = new QLabel("Précision du clic", this);
	    VAL_PrecisionClic = new QSpinBox(this);
		VAL_PrecisionClic->setMaximum(20);
		VAL_PrecisionClic->setMinimum(0);
		VAL_PrecisionClic->setValue(5);
		VAL_PrecisionClic->setToolTip("Distance de tolérance en pixel lors d'un clic pour la sélection d'un Astre");
	// Layouts
	    QHBoxLayout* layPrecisionClic = new QHBoxLayout;
		layPrecisionClic->addWidget(VAL_LabPrecisionClic);
		layPrecisionClic->addWidget(VAL_PrecisionClic);
	// 4: PRECISION ZOOM
	// widget
	    VAL_LabPrecisionZoom = new QLabel("Précision du zoom", this);
	    VAL_PrecisionZoom = new QDoubleSpinBox(this);
		VAL_PrecisionZoom->setMaximum(10);
		VAL_PrecisionZoom->setMinimum(1.01);
		VAL_PrecisionZoom->setValue(1.1);
		VAL_PrecisionZoom->setSingleStep(0.1);
		VAL_PrecisionZoom->setToolTip("Plus le précision est proche de 1, plus le zoem est précis.");
	// Layouts
	    QHBoxLayout* layPrecisionZoom = new QHBoxLayout;
		layPrecisionZoom->addWidget(VAL_LabPrecisionZoom);
		layPrecisionZoom->addWidget(VAL_PrecisionZoom);
	// 5: MUSIQUE
	// widget
	    qCheminMusique = new QString(GRAVASTRE_CHEMIN_MUSIQUE);
	    VAL_LabMusique = new QLabel(*qCheminMusique, this);
	    VAL_Musique = new QPushButton("Autre musique", this);
		VAL_Musique->setToolTip("Cliquer pour ouvrir l'explorateur de fichier et choisir une musique dans le système de fichier, au format flac, ogg ou wav");
	// Layouts
	    QHBoxLayout* layMusique = new QHBoxLayout;
		layMusique->addWidget(VAL_LabMusique);
		layMusique->addWidget(VAL_Musique);
	// connect
	    connect(VAL_Musique, SIGNAL(clicked()),
		    this, SLOT(choisirCheminMusique()));
    // LAYOUT
    layValeurs->addLayout(layPasFleche);
    layValeurs->addLayout(layPasTabulation);
    layValeurs->addLayout(layPrecisionClic);
    layValeurs->addLayout(layPrecisionZoom);
    layValeurs->addLayout(layMusique);
    groupValeurs->setLayout(layValeurs);

    // OPTIONS
    groupOptions = new QGroupBox("Options", this);
    QHBoxLayout* layOptions = new QHBoxLayout;
	// 5: INVERSER ZOOM
	// widget
	    OPT_InvZoom = new QCheckBox("Inverser le zoom", this);
		OPT_InvZoom->setChecked(false);
		OPT_InvZoom->setToolTip("Inverse le sens du zoom opéré avec la molette de la sourie");
	// Layouts
		layOptions->addWidget(OPT_InvZoom);
	// 6: INVERSION DÉFILEMENT
	// widget
	    OPT_InvDefil = new QCheckBox("Inverser le défilement", this);
		OPT_InvDefil->setChecked(false);
		OPT_InvDefil->setToolTip("Inverse le sens de défilement du cliqué-déposé du bouton droit de la sourie");
	// Layouts
		layOptions->addWidget(OPT_InvDefil);
	// 7: ACTIVATION MUSIQUE
	// widget
	    OPT_ActiverMusique = new QCheckBox("Musique", this);
		OPT_ActiverMusique->setChecked(true);
		OPT_ActiverMusique->setToolTip("Cocher pour laisser à l'utilisateur la possibilité de lancer ou de couper la musique");
	// Layouts
		layOptions->addWidget(OPT_ActiverMusique);
    // LAYOUT OPTIONS
    groupOptions->setLayout(layOptions);


    // NAVIGATION
    QVBoxLayout* layNavigation = new QVBoxLayout;
	layNavigation->addWidget(groupValeurs);
	layNavigation->addWidget(groupOptions);
    return layNavigation;
}






// renvois le layout contenant tous les widgets du groupbox groupPrgm
QHBoxLayout* fenetre::INI_GroupeProgramme() {
    QHBoxLayout* layButton = new QHBoxLayout;
    // BOUTON QUITTER
	P_Quitter = new QPushButton("Quitter sans enregistrer", this);
	layButton->addWidget(P_Quitter);
	connect(P_Quitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    // BOUTON ENREGISTRER ET QUITTER
	P_EnrEtQuit = new QPushButton("Enregistrer et Quitter", 
					this);
	layButton->addWidget(P_EnrEtQuit);
	connect(P_EnrEtQuit, SIGNAL(clicked()), 
		this, SLOT(EnregistrerEtQuitter()));
    // BOUTON ENREGISTRER
	P_Enregistrer = new QPushButton("Enregistrer cette configuration", 
					this);
	layButton->addWidget(P_Enregistrer);
	connect(P_Enregistrer, SIGNAL(clicked()), 
		this, SLOT(ecritureFichier()));
    return layButton;
}




