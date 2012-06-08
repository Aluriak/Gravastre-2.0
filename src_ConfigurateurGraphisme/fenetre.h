#ifndef FENETRE_H_INCLUDED
#define FENETRE_H_INCLUDED

#include "main.h"




class fenetre : public QWidget {
    Q_OBJECT

    public:
    // CONSTRUCTEUR
	fenetre(QWidget* = 0);

    // METHODES

    public slots :
        void ecritureFichier(); // écriture dans le fichier cible suivant les états et paramètres choisis par l'utilisateur
	void changementResolution(); // modifie le contenu ou la disponibilité des widgets ECRAN_ChoixTaille[XY] selon le widget ECRAN_ChoixTaille
	void choisirCouleurInterface(); // Demande une couleur à l'utilisateur, puis utilise cette couleur comme couleur d'interface
	void choisirCheminPolice(); // demande un chemin vers une police de l'utilisateur, et la sauvegarde dans le QString qCheminPolice
	void choisirCheminMusique(); // demande un chemin vers une musique de format lisible par la SFML, et le sauvegarde dans la QString qCheminMusique

    private:
    // ATTRIBUTS
	QGroupBox* groupPrgm;
	    QPushButton* P_Quitter;
	    QPushButton* P_Enregistrer;

	QGroupBox* groupGraphique;
	    QGroupBox* groupEcran;
		QLabel* ECRAN_LabTailleEcran;
		QComboBox* ECRAN_TailleEcran;
		QSpinBox* ECRAN_TailleEcranX;
		QSpinBox* ECRAN_TailleEcranY;
		QLabel* ECRAN_LabDefinition;
		QComboBox* ECRAN_Definition;
		QLabel* ECRAN_LabFPS;
		QSpinBox* ECRAN_FPS;
		QLabel* ECRAN_LabAnticrenelage;
		QSpinBox* ECRAN_Anticrenelage;
	    QGroupBox* groupAffichage;
		QLabel* AFF_LabBordSelection;
		QSpinBox* AFF_BordSelection;
		QLabel* AFF_LabTaillePolice;
		QSpinBox* AFF_TaillePolice;
		QLabel* AFF_LabPolice;
		QPushButton* AFF_Police;
		    QString* qCheminPolice; // chemin vers la police
		QLabel* AFF_LabCouleurInterface;
		QPushButton* AFF_CouleurInterface;
		    QColor* qCouleurInterface; // couleur de l'interface

	QGroupBox* groupNavigation;
	    QGroupBox* groupValeurs;
		QLabel* VAL_LabPasFleche;
		QSpinBox* VAL_PasFleche;
		QLabel* VAL_LabPasTabulation;
		QSpinBox* VAL_PasTabulation;
		QLabel* VAL_LabPrecisionClic;
		QSpinBox* VAL_PrecisionClic;
		QLabel* VAL_LabPrecisionZoom;
		QDoubleSpinBox* VAL_PrecisionZoom;
		QLabel* VAL_LabMusique;
		QPushButton* VAL_Musique;
		    QString* qCheminMusique; // chemin vers la musique
	    QGroupBox* groupOptions;
		QCheckBox* OPT_InvZoom;
		QCheckBox* OPT_InvDefil;
		//TODO: QCheckBox* OPT_DesactiverMusique;

    // INITIALISATION
	void initialisationFenetre();
	QVBoxLayout* INI_GroupeGraphique(); // renvois le layout contenant tous les widgets du groupbox groupGraphique
	QVBoxLayout* INI_GroupeNavigation(); // renvois le layout contenant tous les widgets du groupbox groupNavigation
	QHBoxLayout* INI_GroupeProgramme(); // renvois le layout contenant tous les widgets du groupbox groupPrgm
};



#endif // FENETRE_H_INCLUDED
