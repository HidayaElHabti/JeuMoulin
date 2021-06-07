#ifndef JOUEURHUMAIN_H
#define JOUEURHUMAIN_H

#define JOUEUR_HUMAIN_ID 1

#include <iostream>
#include "Joueur.h"

using namespace std;

class JoueurHumain : public Joueur
{
public:
    //Constructeur
    JoueurHumain();

    //Getters
    int getID();
};

#endif // JOUEURHUMAIN_H
