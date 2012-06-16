#ifndef BOITEINFOASTRE_H_INCLUDED
#define BOITEINFOASTRE_H_INCLUDED


class BoiteInfoAstre {
    public:
    // CONSTRUCTEUR
	BoiteInfoAstre(sf::RenderWindow*, sf::Color, float, float, int, int, sf::Font*, int); 

    // METHODES
	void Draw(Astre *); // affiche tous les objets drawables sur app
	void addPosition(float, float); // joute les valeurs envoyées à la position du coin haut-gauche

    // ACCESSEURS
	sf::Vector2f GetPosition(); // renvois les posotion du coin haut-gauche

    private:
    // ATTRIBUTS
	// références
	sf::RenderWindow *app; // on y dessine, et on obtient quelques valeurs importantes
	sf::Font *police; // police d'écriture
	// valeurs graphiques constantes
	int SFML_LargeurBoite; // largeur de la boite en px
	int SFML_HauteurBoite; // hauteur de la boite en px
	int taillePolice; // taille de la police de caractère
	sf::Color clrCadre; // couleur du cadre
	// valeurs graphiques calculées
	sf::Vector2f AFF_Position; // position du coin haut-hauche de la boite
	// affichages graphiques
	sf::String donnees[8]; // données de l'astre
	sf::Shape cadre; // cadre contenant les données
	sf::Image varCroit; // flèche allant vers le haut
	sf::Image varDecroit; // flèche allant vers le bas
	    bool problemeImage; // indicateur sur la capacité à utiliser les images (on les utilise toutes, ou aucune)
	float tabEvolValeurs[6]; // dernières valeurs, servant à déterminer si chaque valeur est en croissance ou décroissance (et agir sure les boléens de variation en conséquence
	bool tabVariation[6]; // vrai si croît, faux sinon. Sert à l'affichage des flèches pour les variations de positions, vitesses et accélérations
};



#endif
