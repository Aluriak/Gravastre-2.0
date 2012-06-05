#ifndef FENETRE_H_INCLUDED
#define FENETRE_H_INCLUDED

#include "main.h"

class fenetre : public QWidget {
    Q_OBJECT

    public:
        fenetre();
        void EcritureFichier();

    private slots :
        void ChangementECH();
        void ChangementPas(int nvPas);
        void Lancer_PRGM();

    private:
        QPushButton *e_LancerSDL;
        QPushButton *e_Quit;
        QSlider *e_ChoixPas;
        QComboBox *e_Curseur;
            QLabel *e_LabelCurseur;
        QComboBox *e_ChoixConfig;
        QPushButton *e_ECH;
        bool ECH_presente;
        QLabel *e_AffichePas;
        QLabel *e_LabelConfig;
};



#endif // FENETRE_H_INCLUDED
