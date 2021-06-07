#include "JoueurHumain.h"

JoueurHumain::JoueurHumain() : Joueur(JOUEUR_HUMAIN_ID)
{

}

int JoueurHumain::getID(){
    return JOUEUR_HUMAIN_ID;
}
