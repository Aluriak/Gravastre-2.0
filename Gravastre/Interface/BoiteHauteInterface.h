#ifndef BOITEHAUTEINTERFACE_H_INCLUDED
#define BOITEHAUTEINTERFACE_H_INCLUDED


class BoiteHauteInterface {
    public:
    // CONSTRUCTEUR
	BoiteHauteInterface(sf::RenderWindow*, sf::Font*, int,
			    sf::Color, int, int, int, int);

    // METHODES
	void Draw(std::string tampon, std::string directive); // Dessine la barre dans app suivant les arguments envoyés

    // ACCESSEURS
    

    private:
    // ATTRIBUTS
	sf::RenderWindow *app; // écran
	int hauteur; // hauteur de la barre en px
	int longueur; // longueur de la barre en px
	sf::Vector2f AFF_Position; // coordonnées du coin haut-gauche
	// valeurs graphiques
	sf::Shape cadre; // cadre principal
	sf::Font *police;
	int taillePolice;
};


#endif
