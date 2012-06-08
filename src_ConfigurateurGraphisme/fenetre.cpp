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
// �criture dans le fichier cible suivant les �tats et param�tres choisis par l'utilisateur
void fenetre::ecritureFichier() {
    // ouverture du fichier cible, + gestion erreur
    ofstream fic(FICHIER_CIBLE); // on supprime ce qu'il y avait avant
    if(!fic) {
	ofstream err("Erreur.txt", ios::app);
	err << "Erreur : le fichier cible n'a pas pu �tre ouvert" << endl;
	err.close();
	return;
    }
    // Variables interm�diaires
    bool inverserZoom = false;
    bool inverserDefil = false;
    if(OPT_InvZoom->isChecked()) inverserZoom = true;
    if(OPT_InvDefil->isChecked()) inverserDefil = true;
    int definition = 32;
    if(ECRAN_Definition->currentText() == DEFINITION_16)
	definition = 16;
    if(ECRAN_Definition->currentText() == DEFINITION_8)
	definition = 8;

    // on va �crire enti�rement le fichier
    // PREMIERE LIGNE
	// Taille de l'�cran
	    fic << ECRAN_TailleEcranX->value()
	    <<":"<< ECRAN_TailleEcranY->value()
	// Taille de le boite d'info astre (constante)
	    <<":"<< 250
	    <<":"<< 250
	// Hauteur de la barre haute d'interface (constante)
	    <<":"<< 20
	// D�finition
	    <<":"<< definition
	// Couleur de l'interface
	    <<":"<< qCouleurInterface->red()
	    <<":"<< qCouleurInterface->green()
	    <<":"<< qCouleurInterface->blue()
	// Anticr�nelage
	    <<":"<< ECRAN_Anticrenelage->value()
	// FPS
	    <<":"<< ECRAN_FPS->value()
	// Tol�rance de s�lection
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
	    <<":"<< inverserZoom
	    <<":"<< inverserDefil
    // QUATRIEME LIGNE
	<<":"<<endl<< qCheminMusique->toStdString() << ":";


    // le fichier � �t� totalement �crit. On le ferme
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





// Demande une couleur � l'utilisateur, puis utilise cette couleur comme couleur d'interface
void fenetre::choisirCouleurInterface() {
    *qCouleurInterface = QColorDialog::getColor(*qCouleurInterface, this);
	QPalette palette(*qCouleurInterface);
	AFF_CouleurInterface->setPalette(palette);
}


// demande un chemin vers une police de l'utilisateur, et la sauvegarde dans le QString qCheminPolice
void fenetre::choisirCheminPolice() {
    QString inter = QFileDialog::getOpenFileName(this, "Choisir la police", QString(), "Polices (*.ttf)");
    if(inter == *qCheminPolice) {
	QMessageBox::information(this, "Police", "Vous avez conserv� la police choisie utl�rieurement");
    } else {
	QMessageBox::information(this, "Police", "Vous avez choisi la police " + inter);
	*qCheminPolice = inter;
	AFF_LabPolice->setText(inter);
    }
}


// demande un chemin vers une musique de format lisible par la SFML, et le sauvegarde dans la QString qCheminMusique
void fenetre::choisirCheminMusique() {
    QString inter = QFileDialog::getOpenFileName(this, "Choisir la musique", QString(), "Musiques (*.flac *.wav *.ogg)");
    if(inter == *qCheminMusique) {
	QMessageBox::information(this, "Musique", "Vous avez conserv� la musique choisie utl�rieurement");
    } else {
	QMessageBox::information(this, "Musique", "Vous avez choisi la musique " + inter);
	*qCheminMusique = inter;
	VAL_LabMusique->setText(inter);
    }
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









// Renvois le layout contenant tous les widgets initialis�s et plac�s du groupbox �cran et affichage
QVBoxLayout* fenetre::INI_GroupeGraphique() {
    // ECRAN
    groupEcran = new QGroupBox("Ecran", this);
	QVBoxLayout* layEcran = new QVBoxLayout;
	// 1: TAILLE ECRAN
	// widget
	    ECRAN_LabTailleEcran = new QLabel("Taille de l'�cran", this);
	    ECRAN_TailleEcran = new QComboBox(this);
		ECRAN_TailleEcran->addItem(RESOLUTION_1400_700);
		ECRAN_TailleEcran->addItem(RESOLUTION_1200_700);
		ECRAN_TailleEcran->addItem(RESOLUTION_1024_600);
		ECRAN_TailleEcran->addItem(RESOLUTION_800_600);
		ECRAN_TailleEcran->addItem(RESOLUTION_PERSONNALISEE);
		ECRAN_TailleEcran->setToolTip("Taille en pixel de l'�cran");
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
	    ECRAN_LabDefinition = new QLabel("D�finition des couleurs", this);
	    ECRAN_Definition = new QComboBox(this);
		ECRAN_Definition->addItem(DEFINITION_32);
		ECRAN_Definition->addItem(DEFINITION_16);
		ECRAN_Definition->addItem(DEFINITION_8);
		ECRAN_Definition->setToolTip("Nombres de bits utilis�s pour coder une couleur. A moins d'avoir de s�rieux probl�mes de carte graphique ou un besoin d'�conomiser les nanosecondes, le mode 32 bits est tout indiqu�");
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
		ECRAN_FPS->setToolTip("Nombre d'image par secondes affich�es, et accessoirement le nombre de dt calcul�s dans le m�me temps. 30 c'est bien, 60 c'est tr�s rapide");
	// Layouts
	    QHBoxLayout* layFPS = new QHBoxLayout;
		layFPS->addWidget(ECRAN_LabFPS);
		layFPS->addWidget(ECRAN_FPS);
	// 4: ANTICRENELAGE
	// widget
	    ECRAN_LabAnticrenelage = new QLabel("Niveaux d'anticr�nelage", 
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
    // layouts d'�cran
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
	    AFF_LabBordSelection = new QLabel("Signalisation de la s�lection",
						this);
	    AFF_BordSelection = new QSpinBox(this);
		AFF_BordSelection->setMaximum(10);
		AFF_BordSelection->setMinimum(1);
		AFF_BordSelection->setValue(5);
		AFF_BordSelection->setToolTip("Augmentation de l'astre s�lectionn� en pixel, de fa�on purement visuelle. 2 pixels, c'est peu, 6 pixels, c'est beaucoup");
	// Layouts
	    QHBoxLayout* layBordSelection = new QHBoxLayout;
		layBordSelection->addWidget(AFF_LabBordSelection);
		layBordSelection->addWidget(AFF_BordSelection);
	// 2: TAILLE POLICE ASTRE NOM
	// widget
	    AFF_LabTaillePolice = new QLabel("Taille de la police des astres", 
					    this);
	    AFF_TaillePolice = new QSpinBox(this);
		AFF_TaillePolice->setMaximum(42);
		AFF_TaillePolice->setMinimum(1);
		AFF_TaillePolice->setValue(12);
		AFF_TaillePolice->setToolTip("Taille de la police pour les noms des astres affich�s dans l'Univers. La taille 12 est id�ale");
	// Layouts
	    QHBoxLayout* layTaillePolice = new QHBoxLayout;
		layTaillePolice->addWidget(AFF_LabTaillePolice);
		layTaillePolice->addWidget(AFF_TaillePolice);
	// 3: POLICE UTILISEE
	// widget
	    AFF_LabPolice = new QLabel(GRAVASTRE_CHEMIN_POLICE, this);
	    AFF_Police = new QPushButton("Autre Police", this);
		AFF_Police->setToolTip("Police utilis�e dans le programme");
		    qCheminPolice = new QString(GRAVASTRE_CHEMIN_POLICE);
	// Layouts
	    QHBoxLayout* layPolice = new QHBoxLayout;
		layPolice->addWidget(AFF_LabPolice);
		layPolice->addWidget(AFF_Police);
	// connect
	    connect(AFF_Police, SIGNAL(clicked()),
		    this, SLOT(choisirCheminPolice()));
	// 4: COULEUR INTERFACE
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








// Renvois le layout contenant tous les widgets initialis�s et plac�s du groupbox Valeurs et Options
QVBoxLayout* fenetre::INI_GroupeNavigation() {
    groupValeurs = new QGroupBox("Valeurs", this);
    QVBoxLayout *layValeurs = new QVBoxLayout;
	// 1: PAS FLECHE
	// widget
	    VAL_LabPasFleche = new QLabel("Pas fl�che", this);
	    VAL_PasFleche = new QSpinBox(this);
		VAL_PasFleche->setMaximum(1000);
		VAL_PasFleche->setMinimum(0);
		VAL_PasFleche->setValue(20);
		VAL_PasFleche->setToolTip("D�placement en pixel lors de la pression surla fl�ches du clavier");
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
		VAL_PasTabulation->setToolTip("Nombre d'instants t calcul�s lors de la pression sur la touche tabulation lorsque le tampon est vide ou incorrect");
	// Layouts
	    QHBoxLayout* layPasTabulation = new QHBoxLayout;
		layPasTabulation->addWidget(VAL_LabPasTabulation);
		layPasTabulation->addWidget(VAL_PasTabulation);
	// 3: PRECISION CLIC
	// widget
	    VAL_LabPrecisionClic = new QLabel("Pr�cision du clic", this);
	    VAL_PrecisionClic = new QSpinBox(this);
		VAL_PrecisionClic->setMaximum(20);
		VAL_PrecisionClic->setMinimum(0);
		VAL_PrecisionClic->setValue(5);
		VAL_PrecisionClic->setToolTip("Distance de tol�rance en pixel lors d'un clic pour la s�lection d'un Astre");
	// Layouts
	    QHBoxLayout* layPrecisionClic = new QHBoxLayout;
		layPrecisionClic->addWidget(VAL_LabPrecisionClic);
		layPrecisionClic->addWidget(VAL_PrecisionClic);
	// 4: PRECISION ZOOM
	// widget
	    VAL_LabPrecisionZoom = new QLabel("Pr�cision du zoom", this);
	    VAL_PrecisionZoom = new QDoubleSpinBox(this);
		VAL_PrecisionZoom->setMaximum(10);
		VAL_PrecisionZoom->setMinimum(1.01);
		VAL_PrecisionZoom->setValue(1.1);
		VAL_PrecisionZoom->setSingleStep(0.1);
		VAL_PrecisionZoom->setToolTip("Plus le pr�cision est proche de 1, plus le zoem est pr�cis.");
	// Layouts
	    QHBoxLayout* layPrecisionZoom = new QHBoxLayout;
		layPrecisionZoom->addWidget(VAL_LabPrecisionZoom);
		layPrecisionZoom->addWidget(VAL_PrecisionZoom);
	// 5: MUSIQUE
	// widget
	    qCheminMusique = new QString(GRAVASTRE_CHEMIN_MUSIQUE);
	    VAL_LabMusique = new QLabel(*qCheminMusique, this);
	    VAL_Musique = new QPushButton("Autre musique", this);
		VAL_Musique->setToolTip("Cliquer pour ouvrir l'explorateur de fichier et choisir une musique dans le syst�me de fichier, au format flac, ogg ou wav");
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
    QVBoxLayout* layOptions = new QVBoxLayout;
	// 5: INVERSER ZOOM
	// widget
	    OPT_InvZoom = new QCheckBox("Inverser le zoom", this);
		OPT_InvZoom->setChecked(false);
		OPT_InvZoom->setToolTip("Inverse le sens du zoom op�r� avec la molette de la sourie");
	// Layouts
	    QHBoxLayout* layInvZoom = new QHBoxLayout;
		layInvZoom->addWidget(OPT_InvZoom);
	// 5: Inversion D�filement
	// widget
	    OPT_InvDefil = new QCheckBox("Inverser le d�filement", this);
		OPT_InvDefil->setChecked(false);
		OPT_InvDefil->setToolTip("Inverse le sens de d�filement du cliqu�-d�pos� du bouton droit de la sourie");
	// Layouts
	    QHBoxLayout* layInvDefil = new QHBoxLayout;
		layInvDefil->addWidget(OPT_InvDefil);
    // LAYOUT OPTIONS
    layOptions->addLayout(layInvZoom);
    layOptions->addLayout(layInvDefil);
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
    // BOUTON ENREGISTRER
	P_Enregistrer = new QPushButton("Enregistrer cette configuration", 
					this);
	layButton->addWidget(P_Enregistrer);
	connect(P_Enregistrer, SIGNAL(clicked()), 
		this, SLOT(ecritureFichier()));
    return layButton;
}




