#ifndef ASTRE_H_INCLUDED
#define ASTRE_H_INCLUDED


/*
 * La classe Astre représente un astre, en contenant les données arithmétiques (position, vitesse, accélération), que géométrique (shape Circle)
 * C'est la base du programme !
 */

class Astre {
    public:
    // CONSTRUCTEUR
	Astre(  Univers*, sf::String, 
		float, float, float, float, 
		sf::Color, 
		float = 0, float = 0,
		bool = 1);

    // METHODES
	virtual void calculsdt(std::vector<Astre*>); // calcule les accélérations en fonction des astres envoyés en arguments
	virtual void Move(); // modifie sa vitesse selon accélérations, puis sa position en fonction des vitesses
	void AddAcc(float, float); // ajouter à l'accélération
	void AddPosition(float, float); // ajouter à la position
	void AddVit(float, float); // ajouter à la vitesse

    // ACCESSEURS
	// Généraux
	    Univers *U;
	    virtual sf::Shape GetShape();
	    sf::String GetNom();
	    void SetMasse(float);
	    void AddMasse(float);
	    float GetMasse();
	    void SetDiametre(float);
	    float GetDiametre();
	    void SetSelection(bool);
	    bool GetSelection();
	// Position
	    void SetPosition(float, float);
	    sf::Vector2f GetPosition();
	// Vitesse
	    void SetVitesse(float, float);
		void SetVitX(float);
		void SetVitY(float);
	    sf::Vector2f GetVitesse();
	// Accélération
	    void SetAcceleration(float, float);
		void SetAccX(float);
		void SetAccY(float);
	    sf::Vector2f GetAcceleration();
    
    protected:
    // ATTRIBUTS
	sf::Shape shape;
	sf::String nom;
	sf::Vector2f a_pos;
	sf::Vector2f a_vit;
	sf::Vector2f a_acc;
	sf::Vector2f gainAcc; // c'est cela qu'on augmente quand on touche à l'éccélération, car cette dernière est remise à zéro avant calcul
	float a_masse;
	float a_diam;
	sf::Color a_clr; // couleur majeure
	sf::Color a_bclr; // couleur de bordure
	// attribut extérieur
	bool affichage; // si vrai, affichage en sortie standard
	bool estSelectionne; // vrai si cet Astre doit être mit en avant
};




#endif
