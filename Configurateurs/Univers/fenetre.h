#ifndef FENETRE_H_INCLUDED
#define FENETRE_H_INCLUDED

#include "main.h"




class fenetre : public QWidget {
    Q_OBJECT
    public:
    // CONSTRUCTEUR ET DESTRUCTEUR
	fenetre(QWidget* = 0);
	~fenetre();

    // METHODES

    public slots :
	void selectionAstre(QTableWidgetItem*); // Modifie le pointeur de sélection selon la ligne cliquée
        void ecritureFichier(); // écriture dans le fichier cible suivant les états et paramètres choisis par l'utilisateur
	void enregistrerFermer(); // enregistre et ferme
	void lancerEnregistrer(); // enregistre, lance Gravastre
	void reinitialisationTableAstre(); // supprime tous le contenu de la table des astres
	void creerAstre(); // affiche la fenetre de création d'Astre
	void editerAstre(); // affiche la fenetre de modification de Astre
	void supprimerAstre(); // supprime le astre sélectionné

    private:
    // ATTRIBUTS
	// Stockage
	    std::vector<Astre*> listeAstre;
	    QTableWidgetItem* selection; // pointeur vers élément sélectionné
	// Widgets
	QGroupBox* groupUnivers;
	    QLabel* a_LabConstG;
	    QDoubleSpinBox* a_ConstGMantisse;
	    QLabel* a_LabConstGExposant;
	    QSpinBox* a_ConstGExposant;
	    QLabel* a_LabVitMax;
	    QDoubleSpinBox* a_VitMax;

	QGroupBox* groupAstre;
	    QTableWidget* tableAstre;
	    QGroupBox* groupBouton;
		QPushButton* nouvAstre;
		QPushButton* editAstre;
		QPushButton* suprAstre;
		QPushButton* reiniTable;
		    bool validiteBoutonsAstre;

	QGroupBox* groupPrgm;
	    QPushButton *w_Quitter;
	    QPushButton *w_EnregistrerEtQuitter;
	    QPushButton *w_EnregistrerEtLancer;

    // METHODES
	void actualiserTable(); // recréer la table d'astre selon le vector et gère la validité des boutons
    // INITIALISATION
	void initialisationFenetre();
	void INI_GroupeUnivers(); // créé et ordonne le groupe d'univers
	void INI_GroupeAstre(); // créé et ordonne le groupe d'Astre
	void INI_GroupePrgm(); // créé et ordonne le groupe prgoramme
	void INI_TableAstre(); // Créé et initialisa la table de astre.
};



#endif // FENETRE_H_INCLUDED
