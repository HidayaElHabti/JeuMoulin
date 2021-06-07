#include "Joueur.h"

Joueur::Joueur(int id)
{
    pieces = 9;
    this->id = id;
}

//Getters

int Joueur::getID(){
    return id;
}

int Joueur::getPieces(){
    return pieces;
}

int Joueur::getPiecesSurPlateau(){
    return piecesSurPlateau.size();
}

vector<int> Joueur::getPiecesSurPlateauVector(){
    return piecesSurPlateau;
}

//Setters

void Joueur::incPieces(){
    pieces++;
}



void Joueur::placePieceSurPlateau(int pos){
    if(pieces>0)
    {
        pieces--;
        piecesSurPlateau.push_back(pos);
    }
}

void Joueur::enlevePieceDuPlateau(int pos){
    if(getPiecesSurPlateau()>0)
    {
        piecesSurPlateau.erase(std::find(piecesSurPlateau.begin(), piecesSurPlateau.end(),pos));
    }
}

void Joueur::deplacePieceSurPlateau(int pos1, int pos2){
    enlevePieceDuPlateau(pos1);
    pieces++;
    placePieceSurPlateau(pos2);
}



bool Joueur::estACourtDePieces(){
    return pieces==0;
}

bool Joueur::aDesPiecesUnprotegesSurPlateau(vector<int> piecesProteges){
    for(vector<int>::iterator it = piecesSurPlateau.begin(); it != piecesSurPlateau.end(); ++it)
    {
        if(std::find(piecesProteges.begin(),piecesProteges.end(),*it) == piecesProteges.end())
            return true;
    }
    return false;
}



void Joueur::reinitialiser(){
    piecesSurPlateau.clear();
    pieces=9;
}
