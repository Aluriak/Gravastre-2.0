#include "main.h"

using namespace std;


/*
 * CONSTRUCTEUR
 */
Univers::Univers(bool aff, sf::Font *pol, int tPol) {
    affichage = aff;
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
// Remplis la liste d'astres selon les données brutes
void Univers::INI_Astres() {
    // on créé un lecteur de fichier d'initialisation
    LecteurFichierINI lfi(FILE_AST, ':');
    if(lfi.lecture() <= 0) { // gestion d'erreur en tout genre
	FATAL_ERROR("INI: Le fichier FILE_AST n'a pas été ouvert");
    }

    // on déclare un pointeur vers les résulats, et on vérifie qu'ils ne sont pas corrompus
    vector<vector<string> > *vec = lfi.getValeurs();
    if(estCorrompu(vec))
	FATAL_ERROR("INI: Le fichier FILE_AST est corrompu");

    // la première chose à faire, c'est de mettre la référence spatiale universelle, de classe ReferenceUnivers qui est un Astre par héritage. (et bénéficie du polymorphisme).
    astres.push_back(new ReferenceUnivers(this, police));

    // boucle d'initialisation (on commence à la deuxième ligne, car la première ligne est utilisée par le patron de ligne, décrivant l'ordre suivant :
    //		NOM:X:Y:MASSE:DIAMETRE:RED:GREEN:BLUE:VX:VY:
    for(unsigned int i = 1; i < vec->size(); i++) {
	// on créé la couleur
	sf::Color clr(str2num((*vec)[i][5]), str2num((*vec)[i][6]), str2num((*vec)[i][7])); 
	// on créé l'astre
	astres.push_back(new Astre(this,
		    sf::String((*vec)[i][0], *police, taillePolice),// arg nom
		    str2float((*vec)[i][1]),			// arg X
		    str2float((*vec)[i][2]),			// arg Y
		    str2float((*vec)[i][3]),			// arg masse
		    str2float((*vec)[i][4]),			// arg diametre
		    clr,					// arg couleur
		    str2float((*vec)[i][8]),			// arg VX
		    str2float((*vec)[i][9]),			// arg VY
		    str2float((*vec)[i][10]),			// arg AX
		    str2float((*vec)[i][11]),			// arg AY
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
    // Chaque case doit avoir 12 mots exactement (définitions des Astres)
    if(vec->size() == 0)		return true;
    for(unsigned int i = 0; i < vec->size(); i++) {
	if((*vec)[i].size() != 12) 	return true;
    }
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



