#include "main.h"

using namespace std;




/*
 * CONSTRUCTEUR
 */
fenetre::fenetre(QWidget* parent) : QWidget(parent) {
    setWindowTitle(PRGM_NOM); // titre de la fenetre
    resize(PRGM_LARGEUR,PRGM_HAUTEUR);
    initialisationFenetre();
}



/*
 * DESTRUCTEUR
 */
fenetre::~fenetre() {
    // on libère les astres
    for(int i = listeAstre.size()-1; i>= 0; i--) { 
	delete listeAstre[i];
	listeAstre.pop_back();
    }
}




/*
 * SLOTS
 */
void fenetre::selectionAstre(QTableWidgetItem* item) {
    int row = 0;
    if(item == NULL)
	row = 0;
    else 
	row = item->row();
    if(row <= -1 || row == 0) { 
	selection = NULL;
	validiteBoutonsAstre = false; // les boutons concernés doivent être rendus inutilisables
    } else {
	selection = item;
	validiteBoutonsAstre = true; // les boutons concernés doivent être rendus utilisables
    }
    // validité des boutons
    editAstre->setEnabled(validiteBoutonsAstre);
    suprAstre->setEnabled(validiteBoutonsAstre);
}



void fenetre::ecritureFichier() {
    // ouverture du fichier
    ofstream fic(FICHIER_CIBLE);
    if(!fic) {
	ofstream err("Erreur.txt", ios::app);
	err << "Le fichier cible n'a pas été ouvert" << endl;
	err.close();
	return;
    }
    // première ligne :  G:vitesse max
    fic << a_ConstGMantisse->value()*pow(10, a_ConstGExposant->value()) 
	<<":"<< a_VitMax->value() << ":" << endl;
    // lignes suivantes : les astres !
    Astre *astre = NULL; // pointeur vers l'astre considéré
    for(unsigned int i = 0; i < listeAstre.size(); i++) {
	astre = listeAstre[i];
	fic << astre->getNom()
	    <<":"<<astre->getPosition().x
	    <<":"<<astre->getPosition().y
	    <<":"<<astre->getMasse()
	    <<":"<<astre->getDiametre()
	    <<":"<<astre->getCouleur().r
	    <<":"<<astre->getCouleur().g
	    <<":"<<astre->getCouleur().b
	    <<":"<<astre->getVitesse().x
	    <<":"<<astre->getVitesse().y
	    <<":"<< 0
	    <<":"<< 0
	    <<":"<< endl;
    }
    fic.close();
}


// enregistre et ferme
void fenetre::enregistrerFermer() {
    ecritureFichier();
    qApp->quit();
}


// enregistre, lance Gravastre
void fenetre::lancerEnregistrer() {
    ecritureFichier();
    // lancement de gravastre en processus détaché
    QProcess::startDetached(CHEMIN_GRAVASTRE); 
}




void fenetre::reinitialisationTableAstre() {
    for(int i = listeAstre.size()-1; i>= 0; i--) { 
	delete listeAstre[i];
	listeAstre.pop_back();
    }
    actualiserTable();
}



void fenetre::creerAstre() {
    // création d'un astre supplémentaire
    listeAstre.push_back(new Astre);
    // pas de modèle, suelement le lien vers l'astre créé par défaut
    fenAstre fenCreation(this, listeAstre.back());
    // retourne vrai si l'utilisateur à enregistré avant de quitter, 0 sinon
    if(!fenCreation.exec()) {
	// l'utilisateur a quitté sans vouloir garder l'astre. Donc le dernier astre est libéré et la case de vector supprimée
	delete listeAstre.back();
	listeAstre.pop_back();
    }
    // on lance l'affichage
    actualiserTable();
}



void fenetre::editerAstre() {
    // l'ID est égal à la ligne de l'astre sélectionné, moins 1 (ligne de référence oblige)
    unsigned int ID = -1;
    if(selection == NULL) return; // si pas de sélection, retour
    // si la sélection est dans la table
    if(selection->row() != -1) ID = selection->row() - 1; 
    else return; // sinon, retour
    // on modifie l'astre d'ID voulu
    fenAstre fenEdition(this, listeAstre[ID], listeAstre[ID]);
    fenEdition.exec(); 
    actualiserTable();
}



void fenetre::supprimerAstre() {
    // l'ID est égal à la ligne de l'astre sélectionné, moins 1 (ligne de référence oblige)
    unsigned int ID = -1;
    if(selection == NULL) return; // si pas de sélection, retour
    // si la sélection est dans la table
    if(selection->row() > 0) ID = selection->row() - 1; 
    else return; // sinon, retour
    // on stocke les astres 1 par 1 tant que la cible n'a pas été rencontrée
    vector<Astre*> inter(0);
    unsigned int i = listeAstre.size() -1;
    for(; i > ID; i--) {
	inter.push_back(listeAstre[i]);
	listeAstre.pop_back();
    }
    // i = ID, donc suppression et libération
    delete listeAstre[i];
    listeAstre.pop_back();
    // maintenant on rajoute les astres enlevés à la liste
    for(int i = inter.size()-1; i >= 0; i--) {
	listeAstre.push_back(inter[i]);
    }
    actualiserTable();
}





/*
 * INITIALISATION
 */
// créé et ordonne le groupe univers
void fenetre::INI_GroupeUnivers() {
    // CONSTANTE G
	a_LabConstG = new QLabel("Constante de gravitation universelle : ", 
				this);
	a_ConstGMantisse = new QDoubleSpinBox(this);
	    a_ConstGMantisse->setRange(-10, 10);
	    a_ConstGMantisse->setDecimals(5);
	    a_ConstGMantisse->setValue(6.67);
	    a_ConstGMantisse->setToolTip("Mantisse de la constante de gravitation G, dans la réalité égale à 6.67 USI");
	a_LabConstGExposant = new QLabel("Puissance de 10 : ", this);
	a_ConstGExposant = new QSpinBox(this);
	    a_ConstGExposant->setRange(-100, 100);
	    a_ConstGExposant->setValue(-11);
	    a_ConstGExposant->setToolTip("Puissance de dix à appliquer à la constante de gravitation G, dans la réalité égale à -11");
	QHBoxLayout* layConstG = new QHBoxLayout;
	    layConstG->addWidget(a_LabConstG);
	    layConstG->addWidget(a_ConstGMantisse);
	    layConstG->addWidget(a_LabConstGExposant);
	    layConstG->addWidget(a_ConstGExposant);
    
    // VITESSE MAX
	a_LabVitMax = new QLabel("Vitesse maximale universelle : ", 
				this);
	a_VitMax = new QDoubleSpinBox(this);
	    a_VitMax->setRange(0, 1e100);
	    a_VitMax->setValue(10);
	    a_VitMax->setToolTip("Vitesse maximale des astres avec le point d'origine comme référentiel. \nPlus cette limite est haute, plus les chances d'induire la simulation en erreur sont élevées. (méthode de calcul d'euler oblige) \nDans la réalité, cette valeur est celle de la lumière, 300 000 km.s-1 \nIci, la valeur 10 (pixel/dt) est garante d'un minimum de cohésion");
	QHBoxLayout* layVitMax = new QHBoxLayout;
	    layVitMax->addWidget(a_LabVitMax);
	    layVitMax->addWidget(a_VitMax);

    // LAYOUTS TERMINAUX
    QVBoxLayout* layUnivers = new QVBoxLayout;
	layUnivers->addLayout(layConstG);
	layUnivers->addLayout(layVitMax);
    groupUnivers = new QGroupBox("Constantes universelles", this);
	groupUnivers->setLayout(layUnivers);
}



// contient la table des astre et les boutons liés
void fenetre::INI_GroupeAstre() {
    // initialisation de la table
    INI_TableAstre();

    // Création et mise en forme des boutons
    QHBoxLayout* layBouton = new QHBoxLayout;
	nouvAstre = new QPushButton("Créer un Astre", this);
	editAstre = new QPushButton("Editer l'Astre", this);
	    // aucun astre sélectionné au démarrage
	    editAstre->setEnabled(false); 
	suprAstre = new QPushButton("Supprimer l'Astre", this);
	    suprAstre->setEnabled(false); // idem
	    // la validité des boutons est en false
		validiteBoutonsAstre = false;
	reiniTable = new QPushButton("Réinitialiser la table", this);
	    reiniTable->setToolTip("Retire définitivement tous les astres de la table.");

	layBouton->addWidget(nouvAstre);
	layBouton->addWidget(editAstre);
	layBouton->addWidget(suprAstre);
	layBouton->addWidget(reiniTable);

    // CONNEXIONS
	// Affichage de la fenêtre de nouveau astre
	connect(nouvAstre, SIGNAL(clicked()), 
			this, SLOT(creerAstre())); // affichage création
	// Affichage de la fenêtre de modification du astre sélectionné
	QObject::connect(editAstre, SIGNAL(clicked()), 
			this, SLOT(editerAstre())); 
	// Supression du Système sélectionné
	connect(suprAstre, SIGNAL(clicked()), 
			this, SLOT(supprimerAstre()));
	// Réinitialisation de la table
	connect(reiniTable, SIGNAL(clicked()), 
		this, SLOT(reinitialisationTableAstre()));

    // LAYOUTS TERMINAUX
	// mise en forme et envois dans le groupe astre
	QVBoxLayout* layAstre = new QVBoxLayout;
	    layAstre->addWidget(tableAstre);
	    layAstre->addLayout(layBouton);
	groupAstre = new QGroupBox("Astres", this);
	    groupAstre->setLayout(layAstre);
}



void fenetre::INI_GroupePrgm() {
    w_Quitter = new QPushButton("Fermer sans enregistrer", this);
	connect(w_Quitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    w_EnregistrerEtQuitter = new QPushButton("Fermer et enregistrer", this);
	connect(w_EnregistrerEtQuitter, SIGNAL(clicked()), 
		this, SLOT(enregistrerFermer()));
    w_EnregistrerEtLancer = new QPushButton("Enregistrer et lancer Gravastre", this);
	connect(w_EnregistrerEtLancer, SIGNAL(clicked()), 
		this, SLOT(lancerEnregistrer()));
    QHBoxLayout *layBout = new QHBoxLayout;
	layBout->addWidget(w_Quitter);
	layBout->addWidget(w_EnregistrerEtQuitter);
	layBout->addWidget(w_EnregistrerEtLancer);
    groupPrgm = new QGroupBox("Programme", this);
	groupPrgm->setLayout(layBout);
}



// créé et initialise la table des astres.
void fenetre::INI_TableAstre() {
    // création de la table
    tableAstre = new QTableWidget(1, TABLE_NB_COLONNE_ASTRE);
    // attributs de la table
	tableAstre->setToolTip("Chaque ligne, sauf la première, représente un astre. Les boutons situés sous la table peuvent vous permettre d'agir sur cette dernière.");
    // l'utilisateur ne peut sélectionner que les lignes
        tableAstre->setSelectionBehavior(QAbstractItemView::SelectRows); 
    // Création de la ligne de référence
    QTableWidgetItem* item = NULL;
    for(int y = 0; y < TABLE_NB_COLONNE_ASTRE; y++) {
	// nom
	if(y == 0) item = new QTableWidgetItem("Nom");
	// masse
	if(y == 1) item = new QTableWidgetItem("Masse");
	// diamètre
	if(y == 2) item = new QTableWidgetItem("Diamètre");
	// couleur
	if(y == 3) item = new QTableWidgetItem("Couleur (RGB)");
	// position
	if(y == 4) item = new QTableWidgetItem("Position");
	// vitesse
	if(y == 5) item = new QTableWidgetItem("Vitesse");

	// ALIGNEMENT
	if(y == 0) // si c'est le nom, centré droit.
	    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	else // sinon, au centre à la verticale et à l'horizontale
	    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	// INSERTION DANS LA TABLE
	tableAstre->setItem(0, y, item);
    }

    // sélection de l'astre dans la table
    connect(tableAstre, SIGNAL(itemClicked(QTableWidgetItem*)), 
	    this, SLOT(selectionAstre(QTableWidgetItem*)));
}




/*
 * METHODE PRIVEE
 */
void fenetre::initialisationFenetre() {
    INI_GroupeUnivers();
    INI_GroupeAstre();
    INI_GroupePrgm();
    QVBoxLayout* layFen = new QVBoxLayout(this);
	layFen->addWidget(groupUnivers);
	layFen->addWidget(groupAstre);
	layFen->addWidget(groupPrgm);
}



// recréer la table d'astre selon le vector et gère la validité des boutons
void fenetre::actualiserTable() {
    tableAstre->setRowCount(1); // on alisse seulement la ligne de référence
    // maintenant, on met le bon nombre de lignes
    tableAstre->setRowCount(1+listeAstre.size());
    // et on rajoutes tous les astres
    QTableWidgetItem* item = NULL;
    for(unsigned int i = 0; i < listeAstre.size(); i++) {
	for(int y = 0; y < TABLE_NB_COLONNE_ASTRE; y++) {
	    // nom
	    if(y == 0) item = new QTableWidgetItem(
				    listeAstre[i]->getNom().c_str());
	    // masse
	    if(y == 1) item = new QTableWidgetItem(
				    num2str(listeAstre[i]->getMasse()).c_str());
	    // diamètre
	    if(y == 2) item = new QTableWidgetItem(
				num2str(listeAstre[i]->getDiametre()).c_str());
	    // couleur
	    if(y == 3) item = new QTableWidgetItem(
			    (listeAstre[i]->getCouleur().affichage()).c_str());
	    // position
	    if(y == 4) item = new QTableWidgetItem(
			    (listeAstre[i]->getPosition().affichage()).c_str());
	    // vitesse
	    if(y == 5) item = new QTableWidgetItem(
			    (listeAstre[i]->getVitesse().affichage()).c_str());

	    // ALIGNEMENT
	    if(y == 0) // si c'est le nom, centré droit.
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	    else // sinon, au centre à la verticale et à l'horizontale
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	    // INSERTION DANS LA TABLE
	    tableAstre->setItem(i+1, y, item);
	}
    }
    // simple sécurité : on ne sait pas où sont les nouveaux items !
    selectionAstre(NULL);
}











