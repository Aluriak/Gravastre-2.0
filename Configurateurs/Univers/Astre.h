#ifndef ASTRE_H_INCLUDED
#define ASTRE_H_INCLUDED




class Astre {
    public:
    // CONSTRUCTEUR
	Astre();
	Astre(  std::string Nom, 
		Vector2f pos, Vector2f vit, 
		float masseMantisse, int masseExposant,  
		float diam, Couleur);

    // METHODES
	void AddPosition(float, float); // ajouter à la position
	void AddVit(float, float); // ajouter à la vitesse
	void AddAcc(float, float); // ajouter à l'accélération

    // ACCESSEURS
	// Généraux
	    std::string getNom();
	    void setNom(std::string);
	    float getMasse();
	    void setMasseMantisse(float);
	    float getMasseMantisse();
	    void setMasseExposant(int);
	    int getMasseExposant();
	    void setDiametre(float);
	    float getDiametre();
	    Couleur getCouleur();
	    void setCouleur(Couleur);
	// Position
	    void setPosition(Vector2f);
	    Vector2f getPosition();
	// Vitesse
	    void setVitesse(Vector2f);
	    Vector2f getVitesse();
    
    private:
    // ATTRIBUTS
	std::string nom;
	Vector2f a_pos;
	Vector2f a_vit;
	float a_masseMantisse;
	int a_masseExposant;
	float a_diam;
	Couleur a_clr;
};

#endif
