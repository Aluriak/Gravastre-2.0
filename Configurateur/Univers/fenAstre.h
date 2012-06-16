#ifndef FENSYSTEME_H_INCLUDED
#define FENSYSTEME_H_INCLUDED


/*
Fenetre permettant, si enregistrement, d'envoyer un signal comportant un pointeur vers l'Astre créé par l'utilisateur dans cette fenêtre, à l'aide de widgets divers permettant de créer facilement et précisément un astre

Le constructeur attent deux astres :
Le premier est l'astre à modifier (NULL non accepté)
Le second est l'astre modèle (NULL autorisé : dans ce cas les valeurs sont mises par défaut)
*/
#include "main.h"

class fenAstre : public QDialog {
    Q_OBJECT
    public:
    // CONSTRUCTEUR
	fenAstre(QWidget* parent, Astre*, Astre* = NULL);

    // METHODE
    
    public slots:
	void choisirCouleur(); // choix de couleur pour l'utilisateur
	void envoiAstre(); // Envois l'astre à la fenêtre parente en exploitant le signal "creationAstre"


    private:
    // ATTRIBUTS
	// astre édité (ou NULL si création à partir de rien)
	    Astre* astre;
	// astre modèle
	    Astre* modele;
	// Widgets
	QGroupBox *groupCreation;
	    // Nom
		QLabel *a_LabNom;
		QLineEdit* a_Nom;
	    // diamètre
		QLabel *a_LabDiametre;
		QDoubleSpinBox* a_Diametre;
	    // Masse
		QLabel *a_LabMasse;
		QLabel *a_LabMasseExposant;
		QDoubleSpinBox* a_MasseMantisse;
		QSpinBox* a_MasseExposant;
	    // Couleurs
		QLabel *a_LabCouleur;
		QPushButton* a_Couleur;
		    QColor* qCouleur; // couleur choisie par l'utilisateur
	    // Positions
		QLabel *a_LabPositionX;
		QDoubleSpinBox* a_PositionX;
		QLabel *a_LabPositionY;
		QDoubleSpinBox* a_PositionY;
	    // Vitesses
		QLabel *a_LabVitesseX;
		QDoubleSpinBox* a_VitesseX;
		QLabel *a_LabVitesseY;
		QDoubleSpinBox* a_VitesseY;
		/*
	    // Accélérations
		QLabel *a_LabAccelerationX;
		QDoubleSpinBox* a_AccelerationX;
		QLabel *a_LabAccelerationY;
		QDoubleSpinBox* a_AccelerationY; // */
	QGroupBox *groupPrgm;
	    QPushButton *quitter; // quitter sans rien faire
	    QPushButton *enregistrer; // quitter et ajouter le système créé
    // METHODES
	std::string nomAuHasard(); // retourne un nom au hasard, pioché dans le fichier FICHIER_NOM_ASTRE
    // INITIALISATION
	void INI_fenAstre(); // créé et ordonne la fenetre entière
	void INI_GroupePrgm(); // créé et ordonne le groupe Prgm
	void INI_GroupeCreation(); // créé et ordonne le groupe création
};


#endif
