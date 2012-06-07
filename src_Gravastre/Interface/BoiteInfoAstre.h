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
};



#endif
