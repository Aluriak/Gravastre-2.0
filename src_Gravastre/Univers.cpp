#include "main.h"

using namespace std;


/*
 * CONSTRUCTEUR
 */
Univers::Univers(bool aff, bool expl, sf::Font *pol, int tPol) {
    affichage = aff;
    exemple = expl;
    police = pol;
    taillePolice = tPol;
    INI_Astres(); // initialisation de la liste d'Astres
}



/*
 * DESTRUCTEUR
 */
Univers::~Univers() {
    liberationMemoire();
    if(affichage)
	cout << "MEM: Libération mémoire Univers terminée" <<endl;
}



/*
 * METHODES PUBLIQUES
 */
// fait passer un instant dt à tous les astres
void Univers::passagedt() {
    // pour chaque astre, on fait calculer les modifs d'accélérations.
    for(unsigned int i = 0; i < astres.size(); i++) {
	astres[i]->calculsdt(astres);
    }
    // puis on les applique !
    for(unsigned int i = 0; i < astres.size(); i++) {
	astres[i]->Move();
    }
}


// calcule et renvoie l'attraction gravitationnelle entre les deux masses en kilos espacées de d mètres
float Univers::attraction(float m1, float m2, float d) {
    return G * ((m1/d) * (m2/d));
}


// borne la vitesse dans l'intervalle [-BORNE_VITESSE;PRMG_BORNE_VITESSE]
void Univers::borneVitesse(sf::Vector2f *vitesse) {
    if(vitesse->x < -bV)	vitesse->x = -bV;
    else if(vitesse->x > bV)	vitesse->x = bV;
    if(vitesse->y < -bV)	vitesse->y = -bV;
    else if(vitesse->y > bV)	vitesse->y = bV;
}



// supprime l'astre en voyé en argument
void Univers::supprimerAstre(Astre *cible) {
    //DEB:std::cout<<"Suppression engagée"<<std::endl;
    int ID = -1; // correspond à la case du vector où est référencé l'astre
    for(unsigned int i = 0; i < astres.size() && ID == -1; i++) {
	if(astres[i] == cible)	
	    ID = i;
    }
    if(ID == -1) { // erreur ! l'adresse envoyée est fausse !
	FATAL_ERROR("UNV: La suppression d'un astre n'a pas aboutit suite à l'envois d'une adresse non valable", false); 
	return; // on arrête là !
    }
    
    // on recopie les addresses des astres encore valable dans un autre vector, on pop le vector astres puis on le rerempli.
    vector<Astre*> inter(0);
    // on parcours la liste du dernier à celui qui doit être supprimé (exclu)
    for(unsigned int i = astres.size()-1; i > (unsigned int)ID; i--) {
	inter.push_back(astres[i]); // on référence l'astre
	astres.pop_back();
    }

    // maintenant, astres.back() est aussi l'astre recherché. On le delete, on pop, et on remet les astres suivants
    delete astres[ID];
    astres.pop_back();
    for(unsigned int i = 0; i < inter.size(); i++)
	astres.push_back(inter[i]);
    if(affichage)
	std::cout<<"L'astre N°"<<ID<<" a été supprimé"<<std::endl;
}



// supprime tout l'Univers et le réinitialise selon le fichier AST.Alk
void Univers::reinitialiserUnivers() {
    liberationMemoire(); // On libère les astres
    INI_Astres(); // et on les reprend !
}




/*
 * ACCESSEURS
 */
vector<Astre*> Univers::getAstres() {
    return astres;
}
float Univers::getG() {
    return G;
}
void Univers::setG(float g) {
    G = g;
}
int Univers::getBorneVitesse() {
    return bV;
}
void Univers::setBorneVitesse(int bv) {
    bV = fabs(bv);
}



/*
 * METHODE PRIVEE
 */
// remplis la liste d'astre avec les astres par défaut
void Univers::listeAstreParDefaut() {
    // constantes universelles
	G = 6.67e-11;
	bV = 10;
    // Soleil
	sf::String Soleil("Soleil", *police, taillePolice);
	sf::String Mercure("Mercure", *police, taillePolice);
	sf::String Venus("Venus", *police, taillePolice);
	sf::String Terre("Terre", *police, taillePolice);
	sf::String Lune("           et Lune", *police, taillePolice);
	sf::String Mars("Mars", *police, taillePolice);
	sf::String Chauffard("Chauffard", *police, taillePolice);
	astres.push_back(new Astre(this, Soleil, 122,0,1e13,14,sf::Color(255,255,0), 0,0));
    // Mercure
	astres.push_back(new Astre(this, Mercure, 122,120,1e07,4,sf::Color(255,125,0), 2.36,0));
    // Vénus
	astres.push_back(new Astre(this, Venus, 122,-210,1e07,4,sf::Color(255,0,125), -1.78,0));
    // Terre
	astres.push_back(new Astre(this, Terre, 422,0,1e07,4,sf::Color(0,0,255), 0,-1.49));
    // Lune
	astres.push_back(new Astre(this, Lune, 422.6,0,7e05,2,sf::Color(255,255,255), 0,-1.52334));
    // Mars
	astres.push_back(new Astre(this, Mars, -328,0,1e07,4,sf::Color(255,0,0), 0,1.21));
    // Chauffard
	astres.push_back(new Astre(this, Chauffard, -878,120,1e05,2,sf::Color(0,255,0), 3,0));
}



// Remplis la liste d'astres selon les données brutes
void Univers::INI_Astres() {
    // la première chose à faire, c'est de mettre la référence spatiale universelle, de classe ReferenceUnivers qui est un Astre par héritage. (et bénéficie du polymorphisme).
    astres.push_back(new ReferenceUnivers(this, police));

    // si il est demandé de montrer l'univers par défaut, pas besoin de faire autre chose
    if(exemple) {
	listeAstreParDefaut();
	return;
    }

    // on créé un lecteur de fichier d'initialisation
    LecteurFichierINI lfi(FILE_AST, ':');
    vector<vector<string> > *vec; // vecteur contenant les résultat
    if(lfi.lecture() <= 0) { // gestion d'erreur en tout genre
	FATAL_ERROR("INI: Le fichier FILE_AST n'a pas été ouvert", false);
	listeAstreParDefaut(); // la liste d'astre ne peut être constituée à partir du fichier, on utilise donc les valeurs par défaut
	if(affichage)
	    cout << "INI: \tListe des Astres par défaut chargée" <<endl;
	return;
    } else {
	// on déclare un pointeur vers les résulats, et on vérifie qu'ils ne sont pas corrompus
	vec = lfi.getValeurs();
	if(estCorrompu(vec)) {
	    FATAL_ERROR("INI: Le fichier FILE_AST est corrompu", false);
	    listeAstreParDefaut(); // la liste d'astre ne peut être constituée à partir du fichier, on utilise donc les valeurs par défaut
	    if(affichage)
		cout << "INI: \tListe des Astres par défaut chargée" <<endl;
	    return;
	}
    }

    // la première ligne du fichier contient des valeurs d'Univers
    G = str2float((*vec)[0][0]);
    bV = str2float((*vec)[0][1]);
    
    // boucle d'initialisation (on commence à la deuxième ligne, car la première ligne est utilisée par le patron de ligne, décrivant l'ordre suivant :
    //		NOM:X:Y:MASSE:DIAMETRE:RED:GREEN:BLUE:VX:VY:
    for(unsigned int i = 1; i < vec->size(); i++) {
	// on créé la couleur
	sf::Color clr(str2num((*vec)[i][5]), str2num((*vec)[i][6]), str2num((*vec)[i][7])); 
	// on créé l'astre
	astres.push_back(new Astre(this,
		    sf::String((*vec)[i][0], *police, taillePolice),// arg nom
		    str2float((*vec)[i][1])*CONVERTION_DIST,	// arg X
		    str2float((*vec)[i][2])*CONVERTION_DIST,	// arg Y
		    str2float((*vec)[i][3])*CONVERTION_KG,	// arg masse
		    str2float((*vec)[i][4]),			// arg diametre
		    clr,					// arg couleur
		    str2float((*vec)[i][8])*CONVERTION_VIT,	// arg VX
		    str2float((*vec)[i][9])*CONVERTION_VIT,	// arg VY
		    affichage					// arg bool aff
		    ));
    }
    // affichages
    if(affichage)
	cout << "INI: \tListe des Astres chargée" <<endl;
}


// renvois vrai si le vector en arg() n'est pas de taille attendue pour l'initialisation des Astres
bool Univers::estCorrompu(vector<vector<string> > *vec) {
    // le vector final doit avoir au moins une case (seulement la ligne de référence, donc pas d'astre)
    if(vec->size() == 0)		return true;
    // la ligne de référence contient les 2 valeurs d'univers
    if((*vec)[0].size() != 2)		return true;
    // Chaque case autre que la première doit avoir 12 mots exactement (définitions des Astres)
    for(unsigned int i = 1; i < vec->size(); i++) {
	if((*vec)[i].size() != 12) 	return true;
    }
    // pas de problème, pas de problème
    return false;
    
}



// libère la mémoire prise par la classe Univers
void Univers::liberationMemoire() {
    // libération des astres
    // tant qu'il y a encore des cases
    while(astres.size() > 0) {	
	delete astres.back(); // libération du dernier item
	astres.pop_back(); // libération de la dernière cas
    }
}



