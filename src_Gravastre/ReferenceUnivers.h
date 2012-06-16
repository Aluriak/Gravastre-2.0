#ifndef REFERENCEUNIVERS_H_INCLUDED
#define REFERENCEUNIVERS_H_INCLUDED



class ReferenceUnivers : public Astre {
    public: 
    // CONSTRUCTEUR
	ReferenceUnivers(Univers*, sf::Font*);
	
    // METHODES
	static Astre* creerReference(Univers*, sf::Font*); // renvois un pointeur Astre, correspondant à l'adresse de la ReferenceUnivers.
	virtual void calculsdt(std::vector<Astre*>); // redéfinition de la fonction de calcul d'accélération et de vitesse
	virtual void Move(); // redéfinition de la fonction de déplacement. Pas de mouvement de la par d'une référence, sinon c'ets le bordel
    // ACCESSEURS
	virtual sf::Shape GetShape(); // redéfinition, parceque le shape n'est pas le même ! 

    private:
    // ATTRIBUTS

    // METHODES
};



#endif
