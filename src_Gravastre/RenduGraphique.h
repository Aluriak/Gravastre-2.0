#ifndef GRAPHIQUE_H_INCLUDED
#define GRAPHIQUE_H_INCLUDED


class RenduGraphique {
    public:
    // CONSTRUCTEURS ET DESTRUCTEUR
	RenduGraphique(bool aff=true, bool expl=false);
	~RenduGraphique();

    // METHODES
	// gère l'affichage graphique et la gestion évènementielle
	void boucleMaitresse(); 
    // ACCESSEURS


    private:
    // ATTRIBUTS
	sf::RenderWindow *app; // fenetre d'affichage
	sf::View *vue; // vue utilisée par app
	Univers *U; // Univers utilisé
	BoiteInfoAstre *bia; // boite d'nformation d'astre sélectionné
	BoiteHauteInterface *bhi; // boite d'interface supérieure
	// Interface
	Astre *selection; // pointe vers l'astre sélectionné
	double zoomActuel; // facteur de zoom actuellement affiché
	// valeurs graphiques
	int SFML_TailleEcranX; // taille de la fenetre en px en abscisses
	int SFML_TailleEcranY; // en ordonnées
	int SFML_TailleBoiteInfoX; // taille de la BoiteInfoAstre en X
	int SFML_TailleBoiteInfoY; // taille de la BoiteInfoAstre en Y
	int SFML_HauteurHauteInterface; // hauteur de la barre d'interface
	int SFML_Definition; // nombre de bit pour coder la couleur
	sf::Color SFML_ClrInterface; // couleur de l'interface
	int SFML_NiveauAntiCrenelage; // niveau d'anticrénelage
	int SFML_FPS; // frame per second
	float SFML_BordAstreSelection; // taille des bords de la sélection
	int SFML_TaillePolice; // taille de la police
	int SFML_TaillePolBoiteInfo; // taille de la police pour BoiteInfoAstre
	int SFML_TPolHauteInterface; // taille de la police pour la barre haute
	AFF_TRAJECTOIRE SFML_Trajectoire; // affichage des trajectoire d'astres
	float SFML_EpaisseurPointTrajectoire; // taille en px des point de trjt
	sf::Font police; // police d'écriture
	// valeurs de navigation
	int NAV_PasFleche; // déplacement en px à l'appuis d'une flèche
	float NAV_CoefficientMolette; // proche de 1 = zoom plus précis
	bool NAV_InverserZoom; // vrai si le zoom doit être inversé
	int NAV_PasTab; // nombre de dt sautés avec la touche tab
	float NAV_PrecisionClic; // précision du clic en px
	bool NAV_InverserDefilement; // vrai si le défilement à la sourie doit être inversé
	bool NAV_ActiverMusique; // vrai si l'utilisateur peut écouter et interargir avec la musique
	// valeurs de sortie
	bool affichage; // si vrai, la sortie standard est utilisée
	bool exemple; // vrai si utilisation de l'univers par défaut
	std::string musique; // chemin vers le fichier musique
	std::string tampon; // tampon d'entrée utilisateur
	std::string directive; // directive à afficher dans la barre haute
	char etat[3]; // indique ce qui est en cours de modification

    // METHODE
	void affichageSFML(); // affiche les astres à l'écran
	bool confirmationQuitter(bool); // demande à l'utilisateur s'il souhaite quitter, dans une boite de dialogue.
	void selectionAstre(int x, int y); // utilise les coordonées de la sourie pour estimer quel astre à été cliqué (clic au coordonnées x;y), puis inflige à ce dernier une bordure rouge transparente. (et enlève cette bordure à l'astre pointé avant le clic, si il existe)
	void modificationAstre(bool); // permet à l'utilisateur de modifier un astre au clavier et à la sourie, en remplacant(faux) ou en ajoutant(vrai)
	OBJET_INTERFACE selectionInterface(int x, int y); // renvois vrai si l'interface à été cliquée. Gère les clics sur l'interface.
    // INITIALISATION
	void INI_Valeurs(); // initialise les valeurs de renduGraphique
	void valeursParDefaut(); // initialise les valeurs de renduGraphique selon les valeurs par défaut.
	bool estCorrompu(std::vector<std::vector<std::string> >*); // renvoi vrai si le vector n'a pas le nombre de cases attendu.
    
    /* A VENIR !
    // METHODES FPV
	void gestionEvenement(); // centre nerveux du programme
	void affichageFPV(); // calculs et affichages
    // */
};


#endif



