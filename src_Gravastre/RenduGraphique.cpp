#include "main.h"

using namespace sf;


/*
 * CONSTRUCTEUR
 */
RenduGraphique::RenduGraphique(bool aff, bool expl) {
    affichage = aff;
    exemple = expl;
    selection = NULL; // pas d'astre sélectionné
    zoomActuel = 1; // le zoom de base est égal à 1
    etat[ETAT_GENERAL] = 'N';
    etat[ETAT_AXE] = ' ';
    etat[ETAT_LIMITE] = '\0';
    INI_Valeurs(); // initialisation des valeurs utilisateurs

    // création de l'univers
    U = new Univers(affichage, exemple, &police, SFML_TaillePolice);
    if(affichage) 
	std::cout << "INI: \tUnivers initialisé\n";
    // création de l'écran
    app = new RenderWindow();
    app->Create( 
	    // Tailles et définition de l'écran
	    VideoMode(SFML_TailleEcranX, SFML_TailleEcranY, SFML_Definition), 
	    // nom du programme, flags de modes
	    PRGM_NAME, Style::Close | Style::Resize, 
	    // Profondeur(0), pochoir(0) et anticrénelage(selon utilisateur)
	    WindowSettings(0,0,SFML_NiveauAntiCrenelage));
    if(!app) exit(EXIT_FAILURE);
    if(affichage) 
	std::cout << "INI: \tRenderWindow initialisée\n";
    app->SetFramerateLimit(SFML_FPS);
    // Création de la vue
    Vector2f centre(0,0);
    Vector2f demiTaille(SFML_TailleEcranX/2, SFML_TailleEcranY/2);
    vue = new View(centre, demiTaille);
    app->SetView(*vue);
    if(affichage) 
	std::cout << "INI: \tVue utilisateur créée\n";
    // Création de la boite d'information d'astre sélectionné
    float tmpx = SFML_TailleEcranX - SFML_TailleBoiteInfoX - 30;
    float tmpy = SFML_TailleEcranY - SFML_TailleBoiteInfoY - 30;
    bia = new BoiteInfoAstre(app, SFML_ClrInterface, 
	    tmpx, tmpy,
	    SFML_TailleBoiteInfoX, SFML_TailleBoiteInfoY, 
	    &police, SFML_TaillePolBoiteInfo);
    // Création de la barre supérieure
    //*
    bhi = new BoiteHauteInterface(app, &police, SFML_TPolHauteInterface, 
				  SFML_ClrInterface, 
				  0, 0,	// coordonnées du coin haut-gauche 
				  app->GetWidth(), SFML_HauteurHauteInterface);
    // Fini ! 
    if(affichage)
	std::cout << "INI: RenduGraphique initialisé !" << std::endl;
}


/*
 * DESTRUCTEUR
 */
RenduGraphique::~RenduGraphique() {
    // destruction de l'interface
	delete bia; // destruction de l'interface 
	delete bhi; // destruction de l'interface 
    // destruction des objets maîtres
	delete vue; // destruction de la vue
	delete U; // destruction de l'Univers
	delete app; // destruction de l'écran
}




/*
 * METHODES PUBLIQUES
 */
// gère l'affichage graphique et la gestion évènementielle
void RenduGraphique::boucleMaitresse() {
    if(affichage)
	std::cout << "INI: \tBoucle Maîtresse Lancée" <<std::endl<<std::endl;

    // intialisations
	int inter = 0; // variable intermédiaire entière
	std::string stmp(""); // variable intermédiaire de type string
	bool interbool = false; // variable intermédiaire de type bool
	bool pause = false; // vrai quand le programme est en pause
	bool suivreAstre = false; // vrai quand on suit l'astre sélectionné
	const Input& input = app->GetInput(); // contient les stattus des touches et boutons en temps réel.
    // variables relatives à la sourie
	bool sourisDroit = false, sourisGauche = false; // vrai si appuyé
	bool boiteInfoBougee = false; // vrai quand la BoiteInfoAstre est ferrée
	Vector2f posSourie(0,0); // coorodnnées de la sourie par rapport 
				 //     au dernier endroit cliqué droit
    // Zoom
	double calcZoom; // utilisée pour calculer les zooms
    // Musique
	int statMsq = 1; // mit à faux si la musique n'est pas utilisable
	Music msq;
	if(NAV_ActiverMusique) { // si musique autorisée
	    if(!msq.OpenFromFile(musique)) {
		FATAL_ERROR("INI: Le fichier de musique n'a pas été ouvert", false);
		statMsq = 0; // on ne doit pas agir sur la musique !
	    } else {
		msq.Play();
		msq.SetLoop(true); // on joue la musique en boucle
	    }
	} else
	    statMsq = 0;
    Event event; // évènement

    // boucle évènementielle
    while(app->IsOpened()) {
	// si on est pas en pause, on passe au dt suivant !
	directive = "SOURIS: deplacer ou selectionner, M: Arreter la musique, A: modifier astre selectionne, ESPACE: changer referentiel, ESC: quitter";
	if(!pause) {
	    U->passagedt();
	    // si un astre est suivis (on suit la sélection), avec sécurité
	    if(suivreAstre && selection != NULL) {
		vue->Move(selection->GetVitesse().x,selection->GetVitesse().y);
		// si on suit un astre, on indique bien ce mode !
		etat[ETAT_GENERAL] = 'R';
	    }
	    else 
		etat[ETAT_GENERAL] = 'N';
	}
	// gestion évènementielle
	while(app->GetEvent(event)) {
	    if(event.Type == Event::Closed)
		app->Close();

	    //===================
	    // TOUCHES DU CLAVIER
	    else if(event.Type == Event::KeyPressed) {
		switch(event.Key.Code) {
		    case Key::Escape: // fin du programme (avec confirmation)
			directive = "Quitter ?  Oui: Escape, Non: Autre touche";
			if(confirmationQuitter(suivreAstre)) app->Close();
			break;
		    case Key::Tab: // on saute le nombre indiqué par le tampon, ou la constante utilisateur NAV_PasTab
			inter = str2num(tampon);
			tampon = ""; // on vide le tampon
			// si rien n'est rentré
			if(inter == 0)
			    inter = NAV_PasTab;
			for(int i = 0; i < inter; i++) 
			    U->passagedt();
			if(affichage)
			    std::cout<<inter<<" dt ont été sautés"<<std::endl;
			break;
		    case Key::Return: // on reviens au zoom normal
			// étant donné les problèmes d'imprécision, on préfère une technique admettant une petite incertitude.
			while (fabs(zoomActuel - 1) > 0.09) {
			    if(zoomActuel < 1) {
				zoomActuel *= NAV_CoefficientMolette;
				vue->Zoom(NAV_CoefficientMolette);
			    }
			    else if(zoomActuel > 1) {
				zoomActuel /= NAV_CoefficientMolette;
				vue->Zoom(1/NAV_CoefficientMolette);
			    } 
			} 
			// de plus, on recentre la fenêtre sur l'origine
			vue->SetCenter(0,0);
			break;
		    case Key::Space: // suivre l'astre sélectionné
			suivreAstre = false; 
			// si un astre est sélectionné
			if(selection != NULL)
			    suivreAstre = true; // on suit l'astre sélect.
			break;
		    case Key::Back: // on retire la dernière case du tampon
			if(tampon.size() == 0) break;
			stmp = tampon;
			tampon = "";
			for(unsigned int i = 0; i < stmp.size()-1; i++) {
			    tampon += stmp[i];
			}
			break;
		    case Key::Pause: // on met en pause, ou on redémarre
			pause = !pause;
			break;

		    // utilisation des lettres, pour différents effets
		    case Key::M: // pour arrêter ou continuer la musique
			if(statMsq) { // si tout vas bien pour la musique
			    if(msq.GetStatus() != Sound::Playing) 
				msq.Play(); // si en pause, on démarre
			    else // sinon on met en pause
				msq.Pause(); 
			}
			break;
		    case Key::A: // modifier l'astre sélectionné
			// si pas d'astre sélectionné, pas la peine de continuer
			// Si Ctrl+a : on AJOUTE aux valeurs existantes
			// Si a : on REMPLACE les valeurs existantes
			if(input.IsKeyDown(Key::LControl) || // ctrl gauche 
				input.IsKeyDown(Key::LControl)) // ou droit
			    interbool = true; // donc on ajoute
			else // sinon
			    interbool = false; // on remplace
			if(selection != NULL) 
			    modificationAstre(interbool); 
			if(affichage)
			    std::cout<<"Astre en cours de modification..."<<std::endl;
			break;
		    case Key::R: // réinitialiser l'Univers
			U->reinitialiserUnivers();
			// on déselectionne et on se suit plus d'astre
			selection = NULL;
			suivreAstre = false;
			if(affichage)
			    std::cout<<"UNIVERS REINITIALISE !"<<std::endl;
			break;


		    // utilisation des flèches, pour se déplacer dans l'univers
		    // Le nombre de pixel s'adapte au zoom
		    case Key::Up:
			vue->Move(0, -NAV_PasFleche/zoomActuel);
			break;
		    case Key::Right:
			vue->Move(NAV_PasFleche/zoomActuel, 0);
			break;
		    case Key::Down:
			vue->Move(0, NAV_PasFleche/zoomActuel);
			break;
		    case Key::Left:
			vue->Move(-NAV_PasFleche/zoomActuel, 0);
			break;

		    // utilisation des chiffres, pour le tampon
		    case Key::Numpad0:
		    case Key::Num0:
			tampon += '0';
			break;
		    case Key::Numpad1:
		    case Key::Num1:
			tampon += '1';
			break;
		    case Key::Numpad2:
		    case Key::Num2:
			tampon += '2';
			break;
		    case Key::Numpad3:
		    case Key::Num3:
			tampon += '3';
			break;
		    case Key::Numpad4:
		    case Key::Num4:
			tampon += '4';
			break;
		    case Key::Numpad5:
		    case Key::Num5:
			tampon += '5';
			break;
		    case Key::Numpad6:
		    case Key::Num6:
			tampon += '6';
			break;
		    case Key::Numpad7:
		    case Key::Num7:
			tampon += '7';
			break;
		    case Key::Numpad8:
		    case Key::Num8:
			tampon += '8';
			break;
		    case Key::Numpad9:
		    case Key::Num9:
			tampon += '9';
			break;
		    case Key::Comma: // virgule, pour les nombres flottants
		    case Key::Period: // ou le point
			// La SFML ne gère pas le point du keypad
			tampon += '.';
			break;
		    case Key::Subtract: // utile pour faire des négatifs
			tampon += '-';
			break;
		    case Key::Add: // ajoute le tampon au FPS (max 1000, min 1)
			SFML_FPS += str2num(tampon);
			if(SFML_FPS > 1000) SFML_FPS = 1000;
			else if(SFML_FPS < 1) SFML_FPS = 1;
			tampon = "";
			app->SetFramerateLimit(SFML_FPS);
			if(affichage)
			    std::cout << "Nouvelle valeur de FPS : " 
					<< SFML_FPS << std::endl;
			break;

		    // dans tous les autres cas :
		    default:
			break;
		}
	    } // fin gestion appuis de touche

	    //=====================
	    // BOUTONS DE LA SOURIE
	    else if(event.Type == Event::MouseButtonPressed) {
		// on défini les coordonnées du point de clic
		posSourie.x = event.MouseButton.X;
		posSourie.y = event.MouseButton.Y;
		// si c'est le bouton droit
		if(event.MouseButton.Button == Mouse::Right) {
		    sourisDroit = true;
		}
		// si c'est le gauche
		else if(event.MouseButton.Button == Mouse::Left) {
		    sourisGauche = true;
		    // si on n'a pas cliqué sur l'interface
		    OBJET_INTERFACE oi = selectionInterface(event.MouseButton.X,event.MouseButton.Y); // on récupère la sortie
		    if(oi == Dehors) {
			// on sélectionne un astre 
			selectionAstre(event.MouseButton.X,event.MouseButton.Y);
		    } else if(oi == BoiteInfo) {
			// on a cliqué sur la BoiteInfoAstre
			// on indique donc que cette boite peut-être déplacée au mouvement de sourie, jusqu'à relâchement du bouton
			boiteInfoBougee = true;
		    } else if(oi == Barre) { // si clique sur la barre haute
			// rien à faire pour le moment
		    }
		}
	    }
	    else if(event.Type == Event::MouseButtonReleased) {
		// si c'est le bouton droit
		if(event.MouseButton.Button == Mouse::Right)
		    sourisDroit = false;
		// si c'est le gauche
		else if(event.MouseButton.Button == Mouse::Left) {
		    sourisGauche = false;
		    boiteInfoBougee = false; // la boite est déselectionnée, quoiqu'il arrive. 
		}
	    }
	    // mouvement de la molette
	    else if(event.Type == Event::MouseWheelMoved) {
		calcZoom = -(event.MouseWheel.Delta*NAV_CoefficientMolette);
		if(NAV_InverserZoom)
		    calcZoom *= -1;
		// on rapelle que le zoom est une valeur positive
		// 	zoom avant si < 1
		// 	zoom arrière si > 1
		if(calcZoom > 0) calcZoom = 1/calcZoom; // calcZoom < 1
		if(calcZoom < 0) calcZoom = -calcZoom; // calcZoom > 1
		vue->Zoom(calcZoom);
		zoomActuel *= calcZoom;
		if(affichage)
		    std::cout<<"ZOOM: " << zoomActuel << std::endl;
	    } // fin gestion des boutons de la souris

	    //========================
	    // MOUVEMENTS DE LA SOURIE
	    else if(event.Type == Event::MouseMoved) {
		// si la souris bouge et que le bouton droit est appuyé
		if(sourisDroit) {
		    // on bouge la vue d'un nombre de px égal à la différence entre la position de la sourie et la position de la sourie lorsque le clic gauche à été fait
		    // note : on multiplie pas 1/zoom pour que ce dernier agisse sur le déplacement. Plus logique et pratique
		    float tmpx = -(event.MouseMove.X - posSourie.x)/zoomActuel;
		    float tmpy = -(event.MouseMove.Y - posSourie.y)/zoomActuel;
		    if(NAV_InverserDefilement) {
			tmpx *= -1;
			tmpy *= -1;
		    }
		    vue->Move(tmpx, tmpy);
		    // on modifie les coordonnées de la sourie pour éviter d'additionner nombres toujours plus grands
		    posSourie.x = event.MouseMove.X;
		    posSourie.y = event.MouseMove.Y;
		}
		// si la souris bouge et que le bouton gauche est appuyé
		else if(sourisGauche) { 
		    // si on a ferré la BoiteInfoAstre
		    if(boiteInfoBougee) {
			// même technique que pour le clic droit, mais indépendant du zoom (l'interface n'est pas liée au zoom)
			float tmpx = (event.MouseMove.X - posSourie.x);
			float tmpy = (event.MouseMove.Y - posSourie.y);
			bia->addPosition(tmpx, tmpy);
			// on modifie les coordonnées de la sourie pour éviter d'additionner nombres toujours plus grands
			posSourie.x = event.MouseMove.X;
			posSourie.y = event.MouseMove.Y;
			
		    }
		}
	    } // fin de gestion des mouvements de la souris


	} // FIN GESTION EVENEMENT

	// on vide l'écran (couleur noire)
	app->Clear();
	// et on affiche le shmilblick
	affichageSFML();
	// actualisation de l'écran
	app->Display();
    } // fin de la boucle maîtresse
    if(affichage)
	std::cout << "LAB: \tBoucle maîtresse terminée" << std::endl;
}






/*
 * METHODES PRIVEES
 */
// affiche les astres à l'écran
void RenduGraphique::affichageSFML() {
    // CONTENU, UNIVERS
    app->SetView(*vue); // on utilise la vue utilisateur
    // affichage des astres et de leur nom
    for(unsigned int i = 0; i < U->getAstres().size(); i++) {
	app->Draw((U->getAstres())[i]->GetShape());
	app->Draw((U->getAstres())[i]->GetNom());
    }
    // INTERFACE
    // On reviens à une vue normale pour ces dessins, car ils s'impriment par dessus le reste et sont indépendant de la vue utilisée
    app->SetView(app->GetDefaultView()); // on utilise la vue interface
    // affichage de la boite de dialogue en bas à droite, contenant les infos relatives à l'astre sélectionné
    if(selection != NULL) 
	bia->Draw(selection);
    bhi->Draw(tampon, directive, etat); // barre du haut affichée !
}




// demande à l'utilisateur s'il souhaite quitter, dans une boite de dialogue.
bool RenduGraphique::confirmationQuitter(bool suivreAstre) {
    int reponse = -1; // -1 = rien; 0 = non; 1 = oui
    // gestion évènementielle
    Event event;
    while(reponse == -1) {
	// Affichages
	app->Clear(); // nettoyage
	U->passagedt(); // on fait passer un instant t
	// si un astre est suivis (on suit la sélection), avec sécurité
	if(suivreAstre && selection != NULL) {
	    vue->Move(selection->GetVitesse().x,selection->GetVitesse().y);
	}
	affichageSFML(); // affichage
	app->Display(); // actualisation de l'écran
	// boucle d'évènements
	while(app->GetEvent(event)) {
	    if(event.Type == Event::KeyPressed) {
		switch(event.Key.Code) {
		    case Key::Escape: // seule escape permet de quitter
			reponse = 1;
			break;
		    default: // toutes les autres disent non
			reponse = 0;
			break;
		}
	    }
	    // les boutons de la sourie sont pris en compte pour un non
	    else if(event.Type == Event::MouseButtonPressed) 
		reponse = 0;
	} // fin gestion évènement
    }
    return (bool)reponse;
 }




// utilise les coordonées de la sourie pour estimer quel astre à été cliqué (clic au coordonnées x;y), puis inflige à ce dernier une bordure rouge transparente. (et enlève cette bordure à l'astre pointé avant le clic, si il existe)
void RenduGraphique::selectionAstre(int x, int y) {
    // les coordonnées reçues de la sourie sont dépendantes directement de la vue. Par conséquent, on les transforme en coordonées relatives au repère fondamental de l'Univers.
    // La fonction de app attends les coordonnées de la sourie dans la fenetre, et étudie la vue utilisée pour convertir cela en coordonées relatives à l'échelle réelle.
    // On passe à la vue univers
    app->SetView(*vue);
    Vector2f inter = app->ConvertCoords(x, y);
    // on reviens à la vue d'interface graphique
    app->SetView(app->GetDefaultView());
    // initialisation
    int xS = inter.x; // variables égales à x et y.
    int yS = inter.y; // remises à jour pour chaque astre
    // d'abord, on indique à l'astre sélectionné qu'il ne l'est plus
    if(selection != NULL)
	selection->SetSelection(false);
    selection = NULL; // et on désélectionne

    // on détermine quel astre a été cliqué (ou si le clic est enregistré à côté
    for(unsigned int i=0; i < U->getAstres().size() && selection == NULL; i++) {
	// distance au centre maximum pour sélectionner
	float distOk = (NAV_PrecisionClic + ( (U->getAstres())[i]->GetDiametre()/2.0 ));
	float distX = fabs( (U->getAstres())[i]->GetPosition().x - xS );
	if(distX < distOk) {
	    // pour éviter des calculs inutiles, la diistance en ordonnées n'est évaluée que la distance en abscisse est suffisemment petit
	    float distY = fabs( (U->getAstres())[i]->GetPosition().y - yS );
	    if(distY < distOk) {
		selection = (U->getAstres())[i]; // on sélectionne l'astre
		// On fait savoir à l'astre qu'il est sélectionné
		selection->SetSelection(true); 
	    }
	}
    }
    // si aucun astre n'a été trouvé, aucun n'a été sélectionné !
    // On affiche, si demandé, les infos de l'astre
    if(affichage && selection != NULL) {
	std::cout<<" Nom   : " << (std::string)selection->GetNom().GetText() << std::endl;
	std::cout<<" Masse : " << selection->GetMasse() << std::endl;
	std::cout<<" PosXY : " << selection->GetPosition().x << " ; " << selection->GetPosition().y << std::endl;
	std::cout<<" VitXY : " << selection->GetVitesse().x << " ; " << selection->GetVitesse().y << std::endl;
	std::cout<<" AccXY : " << selection->GetAcceleration().x << " ; " << selection->GetAcceleration().y << std::endl << std::endl;
    }
}









// permet à l'utilisateur de modifier un astre au clavier et à la sourie, en ajoutant (booléen vrai), ou en remplacant (booléen faux)
void RenduGraphique::modificationAstre(bool ajout) {
    // gestion évènementielle
    // On procède à une attente de touche parmi :
    // 		-escape (annuler)
    // 		-v (modifier la vitesse)	
    // 		    - x (vitesse en x)
    // 		    - y (vitesse en y)
    // 		-a (modifier l'accélération)	
    // 		    - x (accélération en x)
    // 		    - y (accélération en y)
    // 		-m (modifier la masse)	
    // 		- les chiffres, pour remplir le tampon
    // Pendant ce temps, l'Univers est en pause.
    // initialisation
    bool termine = false;
    std::string stmp; // chaine intermédiaire
    bool AxeX = true; // vrai si l'axe visé est X, faux si Y
	etat[ETAT_AXE] = 'X'; // l'axe X est modifié
	etat[ETAT_GENERAL] = 'A'; // on indique qu'un astre est en cours de modification
    bool clicGauche = false; // vrai si le bouton gauche de la sourie est cliqué

    // Directive à afficher
    directive = "Donnez un nombre puis tapez    M: changer masse, XA: changer accélération en x, YV: changer vitesse en y, ESC: annuler";
    // boucle de modification
    while(!termine) {
	Event event; // évènement
	while(app->GetEvent(event)) {
	    // GESTION EVENEMENT
	    if(event.Type == Event::Closed)
		app->Close();
	    else if(event.Type == Event::KeyPressed) {
		switch(event.Key.Code) {
		    case Key::Escape:
			tampon = ""; 
			termine = true; // stop !
			break;
		    // AJOUT/REMPLACEMENT ACCELERATION
		    case Key::A: 
			// AXE X
			if(AxeX) {
			    if(ajout)
				selection->AddAcc(str2float(tampon),0);
			    else
				selection->SetAccX(str2float(tampon));
			    if(affichage) {
				if(ajout)
				    std::cout << "Ajout Accélération X : ";
				else
				    std::cout<<"Remplacement Accélération X : ";
				std::cout << str2float(tampon) << std::endl;
			    }
			}
			// AXE Y
			else {
			    if(ajout)
				selection->AddAcc(0,str2float(tampon));
			    else
				selection->SetAccY(str2float(tampon));
			    if(affichage) {
				if(ajout)
				    std::cout << "Ajout Accélération Y : ";
				else
				    std::cout<<"Remplacement Accélération Y : ";
				std::cout<< str2float(tampon) << std::endl;
			    }
			}
			tampon = "";
			termine = true; // stop !
			break;
		    // AJOUT/REMPLACEMENT VITESSE
		    case Key::V: 
			// AXE X
			if(AxeX) {
			    if(ajout)
				selection->AddVit(str2float(tampon),0);
			    else
				selection->SetVitX(str2float(tampon));
			    if(affichage) {
				if(ajout)
				    std::cout << "Ajout Vitesse X : ";
				else
				    std::cout << "Remplacement Vitesse X : ";
				std::cout<< str2float(tampon) << std::endl;
			    }
			}
			// AXE Y
			else {
			    if(ajout)
				selection->AddVit(0,str2float(tampon));
			    else
				selection->SetVitY(str2float(tampon));
			    if(affichage) {
				if(ajout)
				    std::cout << "Ajout Vitesse Y : ";
				else
				    std::cout << "Remplacement Vitesse Y : ";
				std::cout<< str2float(tampon) << std::endl;
			    }
			}
			tampon = "";
			termine = true; // stop !
			break;
		    case Key::M: // modification de la masse
			if(ajout)
			    selection->SetMasse(str2float(tampon)
							*CONVERTION_KG);
			else
			    selection->AddMasse(str2float(tampon)
							*CONVERTION_KG);
			if(affichage)
			    std::cout << "Nouvelle Masse : " 
				      << str2float(tampon) << std::endl;
			tampon = ""; 
			termine = true; // stop !
			break;
		    case Key::X:
			AxeX = true;
			etat[ETAT_AXE] = 'X'; // l'axe considéré est indiqué
			break;
		    case Key::Y:
			AxeX = false;
			etat[ETAT_AXE] = 'Y'; // l'axe considéré est modifié
			break;
		    case Key::Back: // on retire la dernière case du tampon
			if(tampon.size() == 0) break;
			stmp = tampon;
			tampon = "";
			for(unsigned int i = 0; i < stmp.size()-1; i++) {
			    tampon += stmp[i];
			}
			break;
		    case Key::Delete: // suppression de l'astre
			U->supprimerAstre(selection);
			selection = NULL;
			termine = true; // stop !

		    // utilisation des chiffres, pour le str2float(tampon)
		    case Key::Numpad0:
		    case Key::Num0:
			tampon += '0';
			break;
		    case Key::Numpad1:
		    case Key::Num1:
			tampon += '1';
			break;
		    case Key::Numpad2:
		    case Key::Num2:
			tampon += '2';
			break;
		    case Key::Numpad3:
		    case Key::Num3:
			tampon += '3';
			break;
		    case Key::Numpad4:
		    case Key::Num4:
			tampon += '4';
			break;
		    case Key::Numpad5:
		    case Key::Num5:
			tampon += '5';
			break;
		    case Key::Numpad6:
		    case Key::Num6:
			tampon += '6';
			break;
		    case Key::Numpad7:
		    case Key::Num7:
			tampon += '7';
			break;
		    case Key::Numpad8:
		    case Key::Num8:
			tampon += '8';
			break;
		    case Key::Numpad9:
		    case Key::Num9:
			tampon += '9';
			break;
		    case Key::Comma: // virgule, pour les nombres flottants
		    case Key::Period: // ou le point
			// La SFML ne gère pas le point du keypad
			tampon += '.';
			break;
		    case Key::Subtract: // utile pour faire des négatifs
			tampon += '-';
			break;
		    case Key::E: // utile pour faire des puissances
			tampon += 'e';
			break;

		    default:
			break;
		}
	    } // fin gestion appuis touche


	    // CLICS DE LA SOURIE
	    // position de l'astre = position du clic gauche
	    else if(event.Type == Event::MouseButtonPressed) {
		// si c'est le bouton gauche
		if(event.MouseButton.Button == Mouse::Left) {
		    clicGauche = true;
		    // si on n'a pas cliqué sur l'interface
		    OBJET_INTERFACE oi = selectionInterface(event.MouseButton.X, event.MouseButton.Y); // on récupère la sortie
		    if(oi == Dehors || BoiteInfo) {
			app->SetView(*vue); // on utilise la vue utilisateur
			Vector2f tmp = app->ConvertCoords(event.MouseButton.X, event.MouseButton.Y); // on trouve les coordonnées réelles du clic de sourie
			// et on les applique à l'astre
			selection->SetPosition(tmp.x, tmp.y);
			// on utilise la vue interface
			app->SetView(app->GetDefaultView()); 
		    }
		}

	    } // juste pour voir si on relâche le bouton gauche
	    else if(event.Type == Event::MouseButtonReleased) {
		if(event.MouseButton.Button == Mouse::Left)
		    clicGauche = false; // bouton gauche relâché
	    }

	    // MOUVEMENTS DE LA SOURIE
	    else if(event.Type == Event::MouseMoved) {
		if(clicGauche) { // si le bouton gauche est enfoncé
		    app->SetView(*vue); // on utilise la vue utilisateur
		    Vector2f tmp = app->ConvertCoords(event.MouseMove.X, event.MouseMove.Y); // on trouve les coordonnées réelles du clic de sourie
		    // et on les applique à l'astre
		    selection->SetPosition(tmp.x, tmp.y);
		    // on utilise la vue interface
		    app->SetView(app->GetDefaultView()); 
		}
	    }
	} // fin gestion évènement secondaire

	// AFFICHAGES
	// on vide l'écran (couleur noire)
	app->Clear();
	// et on affiche le shmilblick
	affichageSFML();
	// actualisation de l'écran
	app->Display();
    } // fin de boucle de modification

    etat[ETAT_GENERAL] = 'N'; // on indique qu'on reviens au mode normal
    etat[ETAT_AXE] = ' ';
}




// retourne l'élément de l'interface sélectionné parmi l'énumération OBJET_INTERFACE, et exécute le code relatif à ces éléments.
OBJET_INTERFACE RenduGraphique::selectionInterface(int x, int y) {
    // priorité à la barre du haut
    // On est dedans si la sourie est dans la partie haute de l'écran
    if(y <= SFML_HauteurHauteInterface)
	return Barre;

    // on étudie maintenant la possibilité que ce soit la BoiteInfoAstre
    int xmin = bia->GetPosition().x - NAV_PrecisionClic;
    int xmax = bia->GetPosition().x +SFML_TailleBoiteInfoX + NAV_PrecisionClic;
    if(x >= xmin && x <= xmax) {
	int ymin = bia->GetPosition().y - NAV_PrecisionClic;
	int ymax = bia->GetPosition().y
		    + SFML_TailleBoiteInfoY + NAV_PrecisionClic;
	if(y >= ymin && y <= ymax) {
	    return BoiteInfo;
	}
    }
    // arrivé ici, la seule chose possible est qu'aucun élément de l'interface 
    // 		n'ait été cliqué
    return Dehors;
}





/*
 * INITIALISATION
 */
// initialise les valeurs de renduGraphique et d'Univers
void RenduGraphique::INI_Valeurs() {
    // on créé un lecteur de fichier d'initialisation
    LecteurFichierINI lfi(FILE_INI, ':');
    // on initialise le vector accueillant les résultats
    std::vector<std::vector<std::string> > *val = lfi.getValeurs();
    // on lui fait lire le fichier FILE_INI avec ':' en séparateur
    if(lfi.lecture() <= 0) {
	FATAL_ERROR("INI: Le fichier FILE_INI n'a pas été ouvert", false);
	valeursParDefaut(); // en l'absence de fichier de configuration, on prend les valeurs par défaut
	return;
    } else {
	// on fait un pointeur vers le résultat
	val = lfi.getValeurs();
	// maintenant, on regarde si le fichier n'est pas corrompu
	if(estCorrompu(val)) {
	    FATAL_ERROR("INI: Le fichier FILE_INI est corrompu", false);
	    valeursParDefaut(); // en l'absence de fichier de configuration, on prend les valeurs par défaut
	    return;
	}
    }

    // le fichier n'est pas corrompu, on peut donc utiliser utiliser le vector sans sourcis
    SFML_TailleEcranX = str2num((*val)[0][0]);
    SFML_TailleEcranY = str2num((*val)[0][1]);
    SFML_TailleBoiteInfoX = str2num((*val)[0][2]);
    SFML_TailleBoiteInfoY = str2num((*val)[0][3]);
    SFML_HauteurHauteInterface = str2num((*val)[0][4]);
    SFML_Definition = str2num((*val)[0][5]);
    SFML_ClrInterface = Color(	str2num((*val)[0][6]), 
				str2num((*val)[0][7]), 
				str2num((*val)[0][8]));
    SFML_NiveauAntiCrenelage = str2num((*val)[0][9]);
    SFML_FPS = str2num((*val)[0][10]);
    SFML_BordAstreSelection = str2float((*val)[0][11]);
    SFML_TaillePolice = str2num((*val)[0][12]);
    SFML_TaillePolBoiteInfo = str2num((*val)[0][13]);
    SFML_TPolHauteInterface = str2num((*val)[0][14]);
    // on essaye d'ouvrir la police
    police = Font();
    // si la chaine est vide
    if((*val)[1][0] == "")
	// on prend comme police la police de base
	police = Font::GetDefaultFont();
    else if(!police.LoadFromFile((*val)[1][0])) {
	// on n'arrête pas le programme, mais on dit que ça a merdé
	FATAL_ERROR("INI: le fichier de police n'a pas été ouvert", false);
	// et on prend comme police la police de base
	police = Font::GetDefaultFont();
    }
    // valeurs de navigation
    NAV_PasFleche = str2num((*val)[2][0]);
    NAV_CoefficientMolette = str2float((*val)[2][1]);
    NAV_PasTab = str2num((*val)[2][2]);
    NAV_PrecisionClic = str2float((*val)[2][3]);
    NAV_InverserZoom = str2num((*val)[2][4]);
    NAV_InverserDefilement = str2num((*val)[2][5]);
    NAV_ActiverMusique = (bool)str2num((*val)[2][6]);
    // Valeurs de sortie
    musique = (*val)[3][0];
}


// initialise les valeurs de renduGraphique selon les valeurs par défaut.
void RenduGraphique::valeursParDefaut() {
    SFML_TailleEcranX = 1000;
    SFML_TailleEcranY = 500;
    SFML_TailleBoiteInfoX = 250;
    SFML_TailleBoiteInfoY = 250;
    SFML_HauteurHauteInterface = 20;
    SFML_Definition = 32;
    SFML_ClrInterface = Color(80, 0, 255); 
    SFML_NiveauAntiCrenelage = 4;
    SFML_FPS = 30;
    SFML_BordAstreSelection = 5;
    SFML_TaillePolice = 12;
    SFML_TaillePolBoiteInfo = 14;
    SFML_TPolHauteInterface = 12;
    // on n'arrête pas le programme, mais on dit que ça a merdé
    FATAL_ERROR("INI: le fichier de police n'a pas été ouvert", false);
    // on prend comme police la police de base
    police = Font::GetDefaultFont();
    // valeurs de navigation
    NAV_PasFleche = 20;
    NAV_CoefficientMolette = 1.1;
    NAV_PasTab = 100;
    NAV_PrecisionClic = 5;
    NAV_InverserZoom = 0;
    NAV_InverserDefilement = 0;
    NAV_ActiverMusique = true;
    // Valeurs de sortie
    musique = "Fichiers/Ressources/WelcomeToNova-Emitremmus.flac";
}




// renvoi vrai si le vector n'a pas le nombre de cases attendu.
bool RenduGraphique::estCorrompu(std::vector<std::vector<std::string> >* vec) {
    // le vector final doit avoir 4 cases
    // 		0: 15 cases (valeurs entières SFML)
    // 		1: 1 case  (chemin vers la police)
    // 		2: 7 cases (valeurs de navigations)
    // 		3: 1 cases (valeurs de sortie)
    if(vec->size() != 4)	return true;
    if((*vec)[0].size() != 15) 	return true;
    if((*vec)[1].size() != 1) 	return true;
    if((*vec)[2].size() != 7) 	return true;
    if((*vec)[3].size() != 1) 	return true;
    return false;
}




