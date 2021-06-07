#ifndef PLATEAU_H
#define PLATEAU_H


// Messages d'erreurs
#define ERROR_VERTIX_NOT_EMPTY "A piece has already been placed in this position."
#define ERROR_OUT_OF_PIECES "You are out of pieces."
#define ERROR_ILLEGAL_MOVE "This move isn't possible."
#define ERROR_ILLEGAL_REMOVE "This piece cannot be removed."

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <array>
#include <chrono>
#include <thread>

#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QSignalMapper>
#include <QMessageBox>
#include <QScrollArea>
#include <QListWidget>
#include <QApplication>

#include "Joueur.h"
#include "JoueurHumain.h"
#include "JoueurIA.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Plateau; }
QT_END_NAMESPACE

using namespace std;


class Plateau : public QWidget
{
    Q_OBJECT


    Ui::Plateau *ui;

    /*
        Modelisation du plateau de jeu
        Le plateau de jeu est représenté sous la forme d'une structure graphique composée de:
            - Sommets (les points d'intersection sur le plateau,
            numérotés de 0 à 23, en rangées de haut en bas et de gauche à droite)
            - Arcs (les connexions entre les points)
        Une entrée dans les sommets du tableau peut être:
            - 0: le point du plateau est vide
            - 1: Le point du plateau est occupé par l'une des pièces du joueur humain
            - 2: Le point du plateau est occupé par l'une des pièces du joueur IA
    */
    std::array<int, 24> sommets = {{0}};

    std::array<std::array<int, 2>, 32> arcs = {{
        {{0,1}},
        {{0,9}},
        {{1,2}},
        {{1,4}},
        {{2,14}},
        {{3,10}},
        {{3,4}},
        {{4,5}},
        {{4,7}},
        {{5,13}},
        {{6,7}},
        {{6,11}},
        {{7,8}},
        {{8,12}},
        {{9,10}},
        {{9,21}},
        {{10,11}},
        {{10,18}},
        {{11,15}},
        {{12,13}},
        {{12,17}},
        {{13,14}},
        {{13,20}},
        {{14,23}},
        {{15,16}},
        {{16,17}},
        {{16,19}},
        {{18,19}},
        {{19,20}},
        {{19,22}},
        {{21,22}},
        {{22,23}}
      }};

    /*
        Modelisation des moulins

        moulinPossible stocke toutes les combinaisons de trois points sur la carte qui peuvent former un moulin

        Ce tableau est utilisé pour détecter les moulins pendant le jeu

        Par exemple, le tableau {0,1,2} dans possibleMillPositions signifie que les points 0, 1 et 2 forment un moulin
    */

    std::array<std::array<int, 3>, 16> moulinPossible = {{
          {{0,1,2}},
          {{0,9,21}},
          {{1,4,7}},
          {{2,14,23}},
          {{3,4,5}},
          {{3,10,18}},
          {{5,13,20}},
          {{6,7,8}},
          {{6,11,15}},
          {{8,12,17}},
          {{9,10,11}},
          {{12,13,14}},
          {{15,16,17}},
          {{16,19,22}},
          {{18,19,20}},
          {{21,22,23}}
        }};

    /*
     * Le vecteur pointsProteges enregistre tous les points formant des moulins et ne peuvent donc pas être supprimés
     */

    vector<int> pointsProteges;

    /*
     * Elements de l'interface graphique
     */


      QPushButton * buttons[24];
      QLabel * rowLabels[7];
      QLabel * colLabels[7];

      QLabel * turnLabel;
      QLabel * gameRulesLabel;
      QLabel * gamesWonLabel;
      QLabel * gamesLostLabel;
      QListWidget * statusList;

      QLabel * turnTextLabel;
      QLabel * gamesWonTextLabel;
      QLabel * gamesLostTextLabel;
      QLabel * statusTextLabel;
      QLabel * gameRulesTextLabel;

      QPushButton * resetButton;

      const char * reglesJeu [3] = {
          "The game has begun!\nYou have blue pieces, the computer has red pieces. You take turns placing pieces on the intersections of the lines on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have placed all of your nine pieces.",
          "Phase 2 has begun!\nYou can now move your pieces to connected free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have three pieces left on the board.",
          "Phase 3 has begun!\nYou can now move your pieces freely to any free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThe game ends when one of the players has no more legal moves or less than three pieces left on the board."
        };

      // Les joueurs

      JoueurHumain * joueurHumain;
      JoueurIA * joueurIA;

      /*
          Variables qui capturent l'état du jeu
              tour: le tour actuel
              phaseJeu: La phase de jeu en cours (1, 2 ou 3) pour chacun des joueurs
              moulinDetecte:
                  - 0 si aucun moulin n'est actuellement détecté
                  - 1 si un moulin formé par les pièces du joueur humain est actuellement détecté
                  - 2 si un moulin formé par les pièces du joueur IA est actuellement détecté
              partiesGagnees: le nombre de parties gagnées par le joueur humain
              partiesPerdues: le nombre de parties que le joueur humain a perdues
       */

        int tour;
        int phaseJeu [2];
        int moulinDetecte;
        int partiesGagnees, partiesPerdues;

    public slots:

         /*
          * Réinitialise le plateau de jeu
          * Cette méthode est connectée au bouton "Restart game"
          */

        void reinitialiserJeu();

        /*
       * Cette méthode est liée aux points d'intersection sur le plateau de jeu (les points où les pièces de jeu peuvent être placées)
       * Il contient la logique de ce qui se passe lorsque l'utilisateur clique sur l'un des points
       *
       * Si un moulin est détecté, le joueur humain sera autorisé à retirer l'une des pièces du joueur IA
       * Si aucun moulin n'est détecté et que la phase de jeu est
       *    1, le joueur sera autorisé à placer une pièce
       *    2, le joueur sera autorisé à déplacer une pièce
       *    3, le joueur sera autorisé à déplacer une pièce librement
       */
        void pointSelectionne(int pos);

    public:
        // Constructeur
        Plateau(QWidget *parent = nullptr);

        //Setteurs
        void incPhaseJeu();
        void incPhaseJeu(Joueur * joueur);
        void incTour();

        /*
         * Définir les méthodes de feuille de style
         * Les feuilles de style créent les effets de survol et rendent l'interface utilisateur plus réactive
         */

        void setPlaceHoverStylesheet();
        void setMoveHoverStylesheet();
        void setRemoveHoverStylesheet();

        /*
       * Méthodes de mise à jour de l'interface utilisateur
       * Ces méthodes sont appelées lorsque l'une des variables d'état du jeu change (par exemple la phase de jeu)
       * Ou un message d'erreur doit être affiché
       */

        void updateTurnLabel(int tour);
        void updateGameRulesLabel(QString str);
        void updateStatusLabel(QString str);
        void updateGamePhaseLabel(QString str);
        void updateGamesWonLabel(QString str);
        void updateGamesLostLabel(QString str);
        void showErrorMessage(QString str);

        /*
       * Méthodes de virage
       *
       * Met à jour le tableau des sommets, appelle la méthode de mise à jour du joueur appropriée, met à jour l'interface utilisateur, crée un message d'état
       * Ces méthodes sont appelées lors du placement, du retrait ou du déplacement d'une pièce
       *
       * Paramètres: pos: La position où une pièce vient d'être placée / retirée
       * pos1: La position d'où une pièce vient d'être déplacée
       * pos2: La position où une pièce vient d'être déplacée
       * player: Le joueur qui vient de placer / retirer / déplacer une pièce
       *
       * Renvoie: vrai si l'action a réussi, faux sinon
       */

        bool poserPiece(int pos, Joueur * joueur);
        bool enleverPiece(int pos, Joueur * joueur);
        bool deplacerPiece(int pos1, int pos2, Joueur * joueur);
        bool deplacerPieceLibrement(int pos1, int pos2, Joueur  * joueur);

       /*
        * Contient la logique du tour de l'IA
        * Appelle les méthodes logiques de l'IA qui déterminent son prochain tour
        */

        void iaTour();

        /*
       * Vérifie si deux positions sur le plateau sont connectées (s'il existe un arc entre elles)
       *
       * Paramètres: pos1: Une position sur le plateau
       * pos2: Une autre position sur le plateau
       *
       * Renvoie: Vrai si pos1 et pos2 sont connectés, faux sinon
       */

        bool sontConnectes(int pos1, int pos2);

      /*
       * Vérifie si un joueur a des coups légaux
       * Cette méthode est appelée pour déterminer si le jeu est terminé
       *
       * Paramètres: joueur: le joueur en cours de vérification
       */

        bool aDesMouvementsValides(Joueur * joueur);

        /*
       * Vérifie si un moulin a été formé
       * Si oui, met à jour la variable moulinDetecte
       * Cette méthode est appelée à chaque fois qu'une pièce est placée
       *
       * Paramètres: pos: la position où une pièce vient d'être placée
       */

        void detecterMoulin(int pos);

        /*
       * Vérifie si un moulin existant a été cassé
       * Si oui, supprimer de protectedPoints
       * Cette méthode est appelée à chaque fois qu'une pièce est déplacée / supprimée
       *
       * Paramètres: pos: La position où une pièce vient d'être déplacée / retirée
       * p: l'ID de joueur du joueur qui vient de déplacer / retirer la pièce
       */

        void verifieSiMoulinEstCasse(int pos, int p);

        /*
       * Termine le jeu
       * Crée un message d'état, met à jour la variable partiesgagnes / partiesperdues, désactive tous les boutons du tableau
       *
       * Paramètres: joueurPerdu: Le joueur qui a perdu la partie
       */

        void finDuJeu(Joueur * joueurPerdu);
};
#endif // PLATEAU_H
