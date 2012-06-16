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
    // on lib�re les astres
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
	validiteBoutonsAstre = false; // les boutons concern�s doivent �tre rendus inutilisables
    } else {
	selection = item;
	validiteBoutonsAstre = true; // les boutons concern�s doivent �tre rendus utilisables
    }
    // validit� des boutons
    editAstre->setEnabled(validiteBoutonsAstre);
    suprAstre->setEnabled(validiteBoutonsAstre);
}



void fenetre::ecritureFichier() {
    // ouverture du fichier
    ofstream fic(FICHIER_CIBLE);
    if(!fic) {
	ofstream err("Erreur.txt", ios::app);
	err << "Le fichier cible n'a pas �t� ouvert" << endl;
	err.close();
	return;
    }
    // premi�re ligne :  G:vitesse max
    fic << a_ConstGMantisse->value()*pow(10, a_ConstGExposant->value()) 
	<<";"<< a_VitMax->value() << ";" << endl;
    // lignes suivantes : les astres !
    Astre *astre = NULL; // pointeur vers l'astre consid�r�
    for(unsigned int i = 0; i < listeAstre.size(); i++) {
	astre = listeAstre[i];
	fic << astre->getNom()
	    <<";"<<astre->getPosition().x
	    <<";"<<astre->getPosition().y
	    <<";"<<astre->getMasse()
	    <<";"<<astre->getDiametre()
	    <<";"<<astre->getCouleur().r
	    <<";"<<astre->getCouleur().g
	    <<";"<<astre->getCouleur().b
	    <<";"<<astre->getVitesse().x
	    <<";"<<astre->getVitesse().y
	    <<";"<< 0
	    <<";"<< 0
	    <<";"<< endl;
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
    // lancement de gravastre en processus d�tach�
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
    // cr�ation d'un astre suppl�mentaire
    listeAstre.push_back(new Astre);
    // pas de mod�le, suelement le lien vers l'astre cr�� par d�faut
    fenAstre fenCreation(this, listeAstre.back());
    // retourne vrai si l'utilisateur � enregistr� avant de quitter, 0 sinon
    if(!fenCreation.exec()) {
	// l'utilisateur a quitt� sans vouloir garder l'astre. Donc le dernier astre est lib�r� et la case de vector supprim�e
	delete listeAstre.back();
	listeAstre.pop_back();
    }
    // on lance l'affichage
    actualiserTable();
}



void fenetre::editerAstre() {
    // l'ID est �gal � la ligne de l'astre s�lectionn�, moins 1 (ligne de r�f�rence oblige)
    unsigned int ID = -1;
    if(selection == NULL) return; // si pas de s�lection, retour
    // si la s�lection est dans la table
    if(selection->row() != -1) ID = selection->row() - 1; 
    else return; // sinon, retour
    // on modifie l'astre d'ID voulu
    fenAstre fenEdition(this, listeAstre[ID], listeAstre[ID]);
    fenEdition.exec(); 
    actualiserTable();
}



void fenetre::supprimerAstre() {
    // l'ID est �gal � la ligne de l'astre s�lectionn�, moins 1 (ligne de r�f�rence oblige)
    unsigned int ID = -1;
    if(selection == NULL) return; // si pas de s�lection, retour
    // si la s�lection est dans la table
    if(selection->row() > 0) ID = selection->row() - 1; 
    else return; // sinon, retour
    // on stocke les astres 1 par 1 tant que la cible n'a pas �t� rencontr�e
    vector<Astre*> inter(0);
    unsigned int i = listeAstre.size() -1;
    for(; i > ID; i--) {
	inter.push_back(listeAstre[i]);
	listeAstre.pop_back();
    }
    // i = ID, donc suppression et lib�ration
    delete listeAstre[i];
    listeAstre.pop_back();
    // maintenant on rajoute les astres enlev�s � la liste
    for(int i = inter.size()-1; i >= 0; i--) {
	listeAstre.push_back(inter[i]);
    }
    actualiserTable();
}





/*
 * INITIALISATION
 */
// cr�� et ordonne le groupe univers
void fenetre::INI_GroupeUnivers() {
    // CONSTANTE G
	a_LabConstG = new QLabel("Constante de gravitation universelle : ", 
				this);
	a_UnitConstG = new QLabel("N.m2.Kg-2", this); 
	a_ConstGMantisse = new QDoubleSpinBox(this);
	    a_ConstGMantisse->setRange(-10, 10);
	    a_ConstGMantisse->setDecimals(5);
	    a_ConstGMantisse->setValue(6.67);
	    a_ConstGMantisse->setToolTip("Mantisse de la constante de gravitation G, dans la r�alit� �gale � 6.67 USI");
	a_LabConstGExposant = new QLabel("Puissance de 10 : ", this);
	a_ConstGExposant = new QSpinBox(this);
	    a_ConstGExposant->setRange(-100, 100);
	    a_ConstGExposant->setValue(-11);
	    a_ConstGExposant->setToolTip("Puissance de dix � appliquer � la constante de gravitation G, dans la r�alit� �gale � -11");
	QHBoxLayout* layConstG = new QHBoxLayout;
	    layConstG->addWidget(a_LabConstG);
	    layConstG->addWidget(a_ConstGMantisse);
	    layConstG->addWidget(a_LabConstGExposant);
	    layConstG->addWidget(a_ConstGExposant);
	    layConstG->addWidget(a_UnitConstG);
    
    // VITESSE MAX
	a_LabVitMax = new QLabel("Vitesse maximale universelle : ", 
				this);
	a_UnitVitMax = new QLabel("Km.s-1", this); 
	a_VitMax = new QDoubleSpinBox(this);
	    a_VitMax->setRange(0, 3e5);
	    a_VitMax->setValue(150);
	    a_VitMax->setToolTip("Vitesse maximale des astres avec le point d'origine comme r�f�rentiel. \nPlus cette limite est haute, plus les chances d'induire la simulation en erreur sont �lev�es. (m�thode de calcul d'euler oblige) \nDans la r�alit�, cette valeur est celle de la lumi�re, 300 000 km.s-1 \nIci, la valeur de 150 km.s-1 est garante d'un minimum de coh�sion");
	QHBoxLayout* layVitMax = new QHBoxLayout;
	    layVitMax->addWidget(a_LabVitMax);
	    layVitMax->addWidget(a_VitMax);
	    layVitMax->addWidget(a_UnitVitMax);

    // LAYOUTS TERMINAUX
    QVBoxLayout* layUnivers = new QVBoxLayout;
	layUnivers->addLayout(layConstG);
	layUnivers->addLayout(layVitMax);
    groupUnivers = new QGroupBox("Constantes universelles", this);
	groupUnivers->setLayout(layUnivers);
}



// contient la table des astre et les boutons li�s
void fenetre::INI_GroupeAstre() {
    // initialisation de la table
    INI_TableAstre();

    // Cr�ation et mise en forme des boutons
    QHBoxLayout* layBouton = new QHBoxLayout;
	nouvAstre = new QPushButton("Cr�er un Astre", this);
	editAstre = new QPushButton("Editer l'Astre", this);
	    // aucun astre s�lectionn� au d�marrage
	    editAstre->setEnabled(false); 
	suprAstre = new QPushButton("Supprimer l'Astre", this);
	    suprAstre->setEnabled(false); // idem
	    // la validit� des boutons est en false
		validiteBoutonsAstre = false;
	reiniTable = new QPushButton("R�initialiser la table", this);
	    reiniTable->setToolTip("Retire d�finitivement tous les astres de la table.");

	layBouton->addWidget(nouvAstre);
	layBouton->addWidget(editAstre);
	layBouton->addWidget(suprAstre);
	layBouton->addWidget(reiniTable);

    // CONNEXIONS
	// Affichage de la fen�tre de nouveau astre
	connect(nouvAstre, SIGNAL(clicked()), 
			this, SLOT(creerAstre())); // affichage cr�ation
	// Affichage de la fen�tre de modification du astre s�lectionn�
	QObject::connect(editAstre, SIGNAL(clicked()), 
			this, SLOT(editerAstre())); 
	// Supression du Syst�me s�lectionn�
	connect(suprAstre, SIGNAL(clicked()), 
			this, SLOT(supprimerAstre()));
	// R�initialisation de la table
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



// cr�� et initialise la table des astres.
void fenetre::INI_TableAstre() {
    // cr�ation de la table
    tableAstre = new QTableWidget(1, TABLE_NB_COLONNE_ASTRE);
    // attributs de la table
	tableAstre->setToolTip("Chaque ligne, sauf la premi�re, repr�sente un astre. Les boutons situ�s sous la table peuvent vous permettre d'agir sur cette derni�re.");
    // l'utilisateur ne peut s�lectionner que les lignes
        tableAstre->setSelectionBehavior(QAbstractItemView::SelectRows); 
    // Cr�ation de la ligne de r�f�rence
    QTableWidgetItem* item = NULL;
    for(int y = 0; y < TABLE_NB_COLONNE_ASTRE; y++) {
	// nom
	if(y == 0) item = new QTableWidgetItem("Nom");
	// masse
	if(y == 1) item = new QTableWidgetItem("Masse");
	// diam�tre
	if(y == 2) item = new QTableWidgetItem("Diam�tre");
	// couleur
	if(y == 3) item = new QTableWidgetItem("Couleur (RGB)");
	// position
	if(y == 4) item = new QTableWidgetItem("Position");
	// vitesse
	if(y == 5) item = new QTableWidgetItem("Vitesse");

	// ALIGNEMENT
	if(y == 0) // si c'est le nom, centr� droit.
	    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	else // sinon, au centre � la verticale et � l'horizontale
	    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	// INSERTION DANS LA TABLE
	tableAstre->setItem(0, y, item);
    }

    // s�lection de l'astre dans la table
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



// recr�er la table d'astre selon le vector et g�re la validit� des boutons
void fenetre::actualiserTable() {
    tableAstre->setRowCount(1); // on alisse seulement la ligne de r�f�rence
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
	    // diam�tre
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
	    if(y == 0) // si c'est le nom, centr� droit.
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	    else // sinon, au centre � la verticale et � l'horizontale
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	    // INSERTION DANS LA TABLE
	    tableAstre->setItem(i+1, y, item);
	}
    }
    // simple s�curit� : on ne sait pas o� sont les nouveaux items !
    selectionAstre(NULL);
}











