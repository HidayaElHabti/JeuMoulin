#ifndef JOUEURIA_H
#define JOUEURIA_H

#define JOUEUR_IA_ID 2

#include "Joueur.h"
#include "JoueurHumain.h"

#include <vector>
#include <ctime>
#include <array>
#include <utility>
#include <tuple>



class JoueurIA : public Joueur
{
    // Ces tableaux contiennent des informations sur le plateau de jeu dont l'IA a besoin pour prendre des décisions
    // Ils sont définis dans le constructeur
    std::array<std::array<int, 3>, 16> moulinPossible;
    std::array<std::array<int, 2>, 32> arcs;




public:
    //Constructeurs
    JoueurIA();
    JoueurIA(std::array<std::array<int, 3>, 16> moulinPossible,std::array<std::array<int, 2>, 32> arcs);

    //Getteurs
    int getID();

    /*
   * Donne toutes les positions connectées avec la position entrée
   * Paramètres: pos1: Une position sur le plateau
   *
   * Renvoie: un tableau de toutes les positions connectées
   */
     vector<int> positionsConnectees(int pos);
     /*
      * Vérifie si une position fait partie d'un moulin ou non
      * Cette méthode est appelée lors de l'enlèvement des pièces
      * Paramètres : sommets : les pièces sur plateau
      *            : pos : la position à vérifier
      * Renvoie : vrai si pos est protégée, faux sinon
      */
     bool pieceProtegee(array<int, 24> sommets, int pos);

     /*
      * Calcule pour un joueur donné le nombre de : moulins complets, à une pièce restante, qui bloquent l'adversaire
      *     moulins complets
      *     moulins à une pièces restante
      *     et les moulins de l'adversaire bloqués par une pièce du joueur
      */
     tuple<int,int,int> nbMoulins(array<int, 24> sommets, int id);
     /*
      * Cette fonction calcule l'heuristique d'un état donnée
      * Elle calcule la somme des pièces sur plateau et les différents types de moulins pour les deux joueurs
      * et soustrat ensuite celle du joueur humain de celle du joueur IA
      * Paramètres : sommets : l'état du plateau
      *
      * Renvoie : la valeur de l'heuristique de cette état
      */
     int heuristique(array<int, 24> sommets);

     /*
      * Ces méthodes génerent les états fils d'un état parent selon la phase du jeu
      * Elles sont utilisées dans les fonctions minimax et alpha-beta
      */
     vector<pair<array<int, 24>,int>> generationDesFils0(array<int, 24> sommets,int id);
     vector<pair<array<int, 24>,pair<int,int>>> generationDesFils1(array<int, 24> sommets ,int id, int piecesIA, int piecesHumain);
     vector<pair<array<int, 24>,pair<int,int>>> generationDesFils2(array<int, 24> sommets ,int id);
     vector<pair<array<int, 24>,pair<int,int>>> generationDesFils3(array<int, 24> sommets, int id);

     /*
      * Les méthodes minimax de chaque phase du jeu
      */
     pair<int,int> miniMaxPhase0(array<int, 24> sommets, int prof, int id);
     pair<int,int> miniMaxPhase1(array<int, 24> sommets,int prof, int id, int piecesIA, int piecesHumain);
     tuple<int,int,int> miniMaxPhase2(array<int,24> sommets, int prof, int id);
     tuple<int,int,int> miniMaxPhase3(array<int, 24> sommets, int prof, int id);

     /*
      * Les méthodes alpha-beta de chaque phase du jeu
      */
     pair<int,int> alphaBetaPhase0(array<int, 24> sommets, int prof, int alpha, int beta, int id);
     pair<int,int> alphaBetaPhase1(array<int, 24> sommets, int prof, int alpha, int beta, int id, int piecesIA, int piecesHumain);
     tuple<int,int,int> alphaBetaPhase2(array<int,24> sommets, int prof, int alpha, int beta, int id);
     tuple<int,int,int> alphaBetaPhase3(array<int, 24> sommets, int prof, int alpha, int beta, int id);

     /*
      * Réinitialise le joueur
      * Cette méthode est appelée lorsque le bouton "Restart game" est enfoncé
      */
     void reinitialiser();
};


#endif // JOUEURIA_H
