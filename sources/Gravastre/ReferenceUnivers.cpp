#include "main.h"

using namespace sf;



/*
 * CONSTRUCTEUR 
 */
ReferenceUnivers::ReferenceUnivers( Univers *U, Font *pol) :
				    Astre(  U, 
					    String(PRGM_NOM_REFERENCE, *pol,10),
					    0.0, 0.0, 0.0, 0.0,
					    Color(0,0,0)) {
    // on créé le shape, pour avoir un truc à afficher    
    // le shape de la référence est un losange blanc
    shape = Shape();
    shape.AddPoint(0, 		0-5, 	Color::White);
    shape.AddPoint(0+5,		0, 	Color::White);
    shape.AddPoint(0, 		0+5, 	Color::White);
    shape.AddPoint(0-5,		0, 	Color::White);
    shape.SetOutlineWidth(2);
}




/*
 * METHODES
 */
// On redéfinie la fonction de calcul dt. Il est hors de question que la référence spatiale bouge !
void ReferenceUnivers::calculsdt(std::vector<Astre*> astres) {
    // on ne fait rien, sinon s'assurer que les variables son bien au bon endroit. Pas d'accélération, pas de vitesse, pas de modif de position.
    a_masse = a_pos.x = a_pos.y = a_vit.x = a_vit.y = a_acc.x = a_acc.y = 0;
    // pas d'autre utilité que faire "propre"
}


// On redéfinie la fonction de mouvement. Il est inutile d'opérer les calculs
void ReferenceUnivers::Move() {
    // on ne fait rien. Que voudriez vous faire ?
}





/*
 * ACCESSEURS
 */
// redéfinition, parceque le shape n'est pas le même ! 
Shape ReferenceUnivers::GetShape() {
    return shape;
}

