#ifndef UNIVERS_H_INCLUDED
#define UNIVERS_H_INCLUDED


class Univers {
    public:
    // CONSTRUCTEUR ET DESTRUCTEUR
	Univers(bool aff, bool expl, AFF_TRAJECTOIRE, sf::Font*, int);
	~Univers();

    // METHODES
	void passagedt(); // fait passer un instant dt à tous les astres
	void borneVitesse(sf::Vector2f*); // borne la vitesse dans l'intervalle [-BORNE_VITESSE;PRMG_BORNE_VITESSE]
	float attraction(float, float, float); // calcule et renvoie l'attraction gravitationnelle entre les deux masses en kilos espacées de d mètres
	void supprimerAstre(Astre*); // supprime l'astre en voyé en argument
	void reinitialiserUnivers(); // supprime tout l'Univers et le réinitialise selon le fichier AST.Alk

    // ACCESSEURS
	std::vector<Astre*> getAstres();
	float getG();
	void setG(float);
	int getBorneVitesse();
	void setBorneVitesse(int);
    
    private:
    // ATTRIBUTS
	std::vector<Astre*> astres; // liste des astres
	// valeurs utilisateurs
	float G; // constante de gravitation universelle
	int bV; // vitesse maximale
	AFF_TRAJECTOIRE trajectoire; // affichage des trajectoires des astres
	bool affTrajectoire; // vrai si on doit afficher une trajectoire au -
	bool affIndication; // si vrai, utilisation de la sortie standard
	bool exemple; // vrai si utilisation de l'univers par défaut
	// valeurs de texte
	sf::Font *police;
	int taillePolice;

    // METHODES
	void listeAstreParDefaut(); // remplis la liste d'astre avec les astres par défaut
	void INI_Astres(); // Remplis la liste d'astre selon les données brutes
	bool estCorrompu(std::vector<std::vector<std::string> >*); // renvois vrai si le vector en arg() n'est pas de taille attendue pour l'initialisation des Astres
	void liberationMemoire(); // libère la mémoire prise par la classe Univers
};

#endif



