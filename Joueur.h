#ifndef JOUEUR_H
#define JOUEUR_H
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Joueur
{
protected:
    // ID de joueur
    int id;
    // Quantité de pièces qu'il reste au joueur
    int pieces;
    // Vecteur contenant les positions des pièces du joueur sur le plateau
    vector<int> piecesSurPlateau;
public:
    //Constructeur
    Joueur(int id);

    //Getters
    int getID();
    int getPieces();
    int getPiecesSurPlateau();
    vector<int> getPiecesSurPlateauVector();

    //Setters
    void incPieces();

    /*
    * Mettre à jour pieces et piecesSurPlateau
    * Ces méthodes sont appelées lorsque le joueur effectue son tour (place une pièce, enlève une pièce, déplace une pièce sur le plateau)
    * Paramètres:
    *       pos: La position où une pièce vient d'être placée / retirée
    *       pos1: La position d'où une pièce vient d'être déplacée
    *       pos2: La position où une pièce vient d'être déplacée
    */
    void placePieceSurPlateau(int pos);
    void enlevePieceDuPlateau(int pos);
    void deplacePieceSurPlateau(int pos1, int pos2);

    /*
    * Vérifie si le joueur a des pièces non protégées sur le plateau
    * Cette méthode est appelée lorsque le joueur adverse forme un moulin
    * Si le joueur n'a aucune pièce non protégée à bord, le joueur adverse ne sera pas autorisé à retirer une pièce
    * Paramètres:
    *       pointsProteges: Tous les points protégés du plateau qui ne peuvent pas être supprimés
    * Renvoie: vrai si le joueur a des pièces non protégées sur le plateau, faux sinon
    */
    bool aDesPiecesUnprotegesSurPlateau(vector<int> piecesProteges);

    /*
    * Vérifie si le joueur est à court de pièces
    * Cette méthode est appelée pour déterminer si toutes les pièces ont été placées et la phase de jeu 1 se termine
    * Renvoie: vrai si le joueur est à court de pièces, faux sinon
    */
    bool estACourtDePieces();

    /*
    * Réinitialise le joueur
    * Cette méthode est appelée lorsque le bouton "Restart game" est clické
    */
    void reinitialiser();
};

#endif // JOUEUR_H
