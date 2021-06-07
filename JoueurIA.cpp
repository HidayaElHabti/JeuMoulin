#include "JoueurIA.h"

JoueurIA::JoueurIA() : Joueur(JOUEUR_IA_ID)
{
    srand(time(NULL));
}

JoueurIA::JoueurIA(std::array<std::array<int, 3>, 16> moulinPossible, std::array<std::array<int, 2>, 32> arcs)
    :Joueur(JOUEUR_IA_ID)
{
    srand(time(NULL));
    this->moulinPossible = moulinPossible;
    this->arcs = arcs;
}

int JoueurIA::getID(){
    return JOUEUR_IA_ID;
}

vector<int> JoueurIA::positionsConnectees(int pos)
{
    vector<int> positions;
    for(int i=0; i<32; i++)
    {
        if(arcs[i][0] == pos)
            positions.push_back(arcs[i][1]);
        if(arcs[i][1] == pos)
            positions.push_back(arcs[i][0]);
    }
    return positions;
}

bool JoueurIA::pieceProtegee(array<int, 24> sommets, int pos)
{
    int point1, point2, point3;

    for(int i = 0; i < 16; i++)
    {
        point1 = moulinPossible[i][0];
        point2 = moulinPossible[i][1];
        point3 = moulinPossible[i][2];

        if(point1 == pos || point2 == pos || point3 == pos)
        {
          if(sommets[point1] != 0 && sommets[point1] == sommets[point2] && sommets[point2] == sommets[point3])
          {
              return true;
          }
        }
    }
    return false;
}

tuple<int,int,int> JoueurIA::nbMoulins(std::array<int, 24> sommets, int id)
{
    tuple<int,int,int> moulin (0,0,0);//nombre de moulins complets, à une pièce restante, qui bloquent l'adversaire
        int nbPieces, nbVide, nbPiecesAdversaire ;
        int point[3];
        for(int i = 0; i < 16; i++)
        {
            nbPieces = 0; nbVide = 0; nbPiecesAdversaire = 0;
            point[0] = moulinPossible[i][0];
            point[1] = moulinPossible[i][1];
            point[2] = moulinPossible[i][2];
            for(int j = 0; j < 3; j++)
            {

                if(sommets[point[j]] == id)
                    nbPieces++;
                else if(sommets[point[j]] == 0)
                    nbVide++;
                else
                    nbPiecesAdversaire ++;
            }
            if(nbPieces == 3)
                get<0>(moulin) ++;
            else if(nbPieces == 2 && nbVide == 1)
                get<1>(moulin) ++;
            else if(nbPiecesAdversaire == 2 && nbPieces == 1)
                get<2>(moulin) ++;

        }
        return moulin;
}

int JoueurIA::heuristique(array<int, 24> sommets)
{
    tuple<int,int,int> scoreIndiceIA, scoreIndiceHumain;

    scoreIndiceIA = nbMoulins(sommets, JOUEUR_IA_ID);
    scoreIndiceHumain = nbMoulins(sommets, JOUEUR_HUMAIN_ID);

    int sommeIA, sommeHumain;

    sommeIA=get<0>(scoreIndiceIA)*50+get<1>(scoreIndiceIA)*20+get<2>(scoreIndiceIA)*10;

    sommeHumain=get<0>(scoreIndiceHumain)*50+get<1>(scoreIndiceHumain)*20+get<2>(scoreIndiceHumain)*10;

    for(int i=0; i<24; i++)
    {
        if(sommets[i] == JOUEUR_IA_ID)
            sommeIA += 10;
        else if(sommets[i] == JOUEUR_HUMAIN_ID)
            sommeHumain += 10;
    }

    return sommeIA - sommeHumain;
}
vector<pair<array<int, 24>,int>> JoueurIA::generationDesFils0(array<int, 24> sommets ,int id)
{
    vector<pair<array<int, 24>,int>> fils;
    //génération des fils
    for (int i = 0; i < 24; i++)
    {
       if (sommets[i] == id && !pieceProtegee(sommets,i))
       {
            sommets[i] = 0;
            fils.push_back(make_pair(sommets,i));
            //on remet le plateau à son état initale
            sommets[i]=id;
        }
    }
    return fils;
}
pair<int,int> JoueurIA::miniMaxPhase0(std::array<int, 24> sommets, int prof, int id)
{
    if(prof == 0)
        return make_pair(heuristique(sommets),-1);

    vector<pair<array<int, 24>,int>> fils;

    fils = generationDesFils0(sommets,id);

    if(id == JOUEUR_IA_ID)
    {
        pair<int,int> nouveau, maxScorePos(-10000,-1);
        vector<pair<array<int, 24>,int>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau = miniMaxPhase0((*it).first,prof-1,id - 1);
            if(maxScorePos.first < nouveau.first)
            {
                maxScorePos.first = nouveau.first;
                maxScorePos.second = (*it).second;
            }
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {
        pair<int,int> nouveau, minScorePos(10000,-1);
        vector<pair<array<int, 24>,int>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau = miniMaxPhase0((*it).first,prof-1,id + 1);
            if(minScorePos.first > nouveau.first)
            {
                minScorePos.first = nouveau.first;
                minScorePos.second = (*it).second;
            }
        }
        return minScorePos;

    }

    return make_pair(-1,-1);
}
pair<int,int> JoueurIA::alphaBetaPhase0(std::array<int, 24> sommets, int prof, int alpha, int beta, int id)
{
    if(prof == 0)
        return make_pair(heuristique(sommets),-1);

    vector<pair<array<int, 24>,int>> fils;

    fils = generationDesFils0(sommets,id);

    if(id == JOUEUR_IA_ID)
    {
        pair<int,int> nouveau, maxScorePos(-10000,-1);
        vector<pair<array<int, 24>,int>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau = alphaBetaPhase0((*it).first,prof-1,alpha,beta,id - 1);
            if(maxScorePos.first < nouveau.first)
            {
                maxScorePos.first = nouveau.first;
                maxScorePos.second = (*it).second;
            }
            if(alpha < maxScorePos.first)
                alpha = maxScorePos.first;
            if(beta <= alpha)
                break;
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {
        pair<int,int> nouveau, minScorePos(10000,-1);
        vector<pair<array<int, 24>,int>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau = miniMaxPhase0((*it).first,prof-1,id + 1);
            if(minScorePos.first > nouveau.first)
            {
                minScorePos.first = nouveau.first;
                minScorePos.second = (*it).second;
            }
            if(beta > minScorePos.first)
                beta = minScorePos.first;
            if(beta <= alpha)
                break;
        }
        return minScorePos;

    }

    return make_pair(-1,-1);
}
vector<pair<array<int, 24>,pair<int,int>>> JoueurIA::generationDesFils1(array<int, 24> sommets, int id, int piecesIA, int piecesHumain)
{
    vector<pair<array<int, 24>,pair<int,int>>> fils;
    if(id == JOUEUR_IA_ID)
    {
        //génération des fils
        for (int i = 0; i < 24; i++)
        {
           if (sommets[i] == 0)
           {
                sommets[i] = JOUEUR_IA_ID;
                piecesIA--;
                fils.push_back(make_pair(sommets,make_pair(piecesIA,i)));
                //on remet le plateau à son état initale
                sommets[i]=0;
                piecesIA++;
            }
        }
    }
    else if(id == JOUEUR_HUMAIN_ID)
    {
        //génération des fils
        for (int i = 0; i < 24; i++)
        {
            if (sommets[i] == 0)
            {
                  sommets[i] = JOUEUR_HUMAIN_ID;
                  piecesHumain--;
                  fils.push_back(make_pair(sommets,make_pair(piecesHumain,i)));
                  //on remet le plateau à son état initale
                  sommets[i]=0;
                  piecesHumain++;
              }
        }
    }
    return fils;
}
pair<int,int> JoueurIA::miniMaxPhase1(std::array<int, 24> sommets, int prof, int id, int piecesIA, int piecesHumain)
{
    if(prof == 0 || (id == JOUEUR_IA_ID && piecesIA == 0) || (id == JOUEUR_HUMAIN_ID && piecesHumain == 0))
        return make_pair(heuristique(sommets),-1);

    vector<pair<array<int, 24>,pair<int,int>>> fils;
    fils = generationDesFils1(sommets,id,piecesIA,piecesIA);

    if(id == JOUEUR_IA_ID)
    {

        pair<int,int> nouveau, maxScorePos(-10000,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau = miniMaxPhase1((*it).first,prof-1,id - 1,((*it).second).first,piecesHumain);
            if(maxScorePos.first < nouveau.first)
            {
                maxScorePos.first = nouveau.first;
                maxScorePos.second = ((*it).second).second;
            }
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {
        pair<int,int> nouveau, minScorePos(10000,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau = miniMaxPhase1((*it).first,prof-1,id + 1,piecesIA,((*it).second).first);
            if(minScorePos.first > nouveau.first)
            {
                minScorePos.first = nouveau.first;
                minScorePos.second = ((*it).second).second;
            }
        }
        return minScorePos;

    }
    return make_pair(-1,-1);
}
pair<int,int> JoueurIA::alphaBetaPhase1(std::array<int, 24> sommets, int prof, int alpha, int beta, int id, int piecesIA, int piecesHumain)
{
    if(prof == 0 || (id == JOUEUR_IA_ID && piecesIA == 0) || (id == JOUEUR_HUMAIN_ID && piecesHumain == 0))
        return make_pair(heuristique(sommets),-1);

    vector<pair<array<int, 24>,pair<int,int>>> fils;
    fils = generationDesFils1(sommets,id,piecesIA,piecesIA);

    if(id == JOUEUR_IA_ID)
    {

        pair<int,int> nouveau, maxScorePos(-10000,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau = alphaBetaPhase1((*it).first,prof-1,alpha,beta,id - 1,((*it).second).first,piecesHumain);
            if(maxScorePos.first < nouveau.first)
            {
                maxScorePos.first = nouveau.first;
                maxScorePos.second = ((*it).second).second;
            }
            if(alpha < maxScorePos.first)
                alpha = maxScorePos.first;
            if(beta <= alpha)
                break;
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {
        pair<int,int> nouveau, minScorePos(10000,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator it;
        for (it = fils.begin(); it != fils.end(); it++)
        {
            nouveau =alphaBetaPhase1((*it).first,prof-1,alpha,beta,id + 1,piecesIA,((*it).second).first);
            if(minScorePos.first > nouveau.first)
            {
                minScorePos.first = nouveau.first;
                minScorePos.second = ((*it).second).second;
            }
            if(beta > minScorePos.first)
                beta = minScorePos.first;
            if(beta <= alpha)
                break;
        }
        return minScorePos;

    }
    return make_pair(-1,-1);
}
vector<pair<array<int, 24>,pair<int,int>>> JoueurIA::generationDesFils2(array<int, 24> sommets, int id)
{
    vector<pair<array<int, 24>,pair<int,int>>> fils;
    cout << id <<endl;
    if(id == JOUEUR_IA_ID)
    {
       // pour chaque piece sur le plateau
       for (int i=0;i<24;i++)
       {
           if(sommets[i]==JOUEUR_IA_ID)
           {
               int pos1=i;
               vector<int> connecte = positionsConnectees(pos1);
               vector<int> ::iterator pt;
               // pour chaque position vide connectee avec pos1
               for (pt = connecte.begin(); pt != connecte.end() ; pt++)
               {
                   int pos2 = *pt;
                   if(sommets[pos2] == 0)
                   {
                       sommets[pos1] = 0;
                       sommets[pos2] = JOUEUR_IA_ID;
                       fils.push_back(make_pair(sommets, make_pair(pos1, pos2)));
                       // on remet le plateau à son état initial
                       sommets[pos1] = JOUEUR_IA_ID;
                       sommets[pos2] = 0;

                   }
                }


            }
        }
    }
    else if(id == JOUEUR_HUMAIN_ID)
    {
       // pour chaque piece sur le plateau
       for(int i=0; i<24;i++)
       {
           if(sommets[i]==JOUEUR_HUMAIN_ID)
           {
               int pos1=i;
               vector<int> connecte = positionsConnectees(pos1);
                vector<int> ::iterator pt;
                // pour chaque position vide connectee avec pos1
               for (pt = connecte.begin(); pt != connecte.end(); pt++)
               {
                   int pos2 = *pt;
                   if(sommets[pos2] == 0)
                   {
                       sommets[pos1] = 0;
                       sommets[pos2] = JOUEUR_HUMAIN_ID;
                       fils.push_back(make_pair(sommets, make_pair(pos1, pos2)));
                       // on remet le plateau à son état initial
                       sommets[pos1] = JOUEUR_HUMAIN_ID;
                       sommets[pos2] = 0;
                   }


               }

           }

       }

    }
    return fils;
}
tuple<int,int,int> JoueurIA::miniMaxPhase2(std::array<int, 24> sommets, int prof, int id)
{
    if(prof == 0 )
        return make_tuple(heuristique(sommets),-1,-1);

    vector<pair<array<int, 24>,pair<int,int>>> fils;
    fils = generationDesFils2(sommets,id);

    if(id == JOUEUR_IA_ID)
    {
        tuple<int,int,int> nouveau, maxScorePos(-10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            nouveau = miniMaxPhase2((*jt).first,prof-1,id - 1);
            if(get<0>(maxScorePos) < get<0>(nouveau))
            {
                get<0>(maxScorePos) = get<0>(nouveau);
                get<1>(maxScorePos) = ((*jt).second).first;
                get<2>(maxScorePos) = ((*jt).second).second;
            }
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {


        tuple<int,int,int> nouveau, minScorePos(10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            nouveau = miniMaxPhase2((*jt).first,prof-1,id + 1);
            if(get<0>(minScorePos) > get<0>(nouveau))
            {
                get<0>(minScorePos) = get<0>(nouveau);
                get<1>(minScorePos) = ((*jt).second).first;
                get<2>(minScorePos) = ((*jt).second).second;
            }
        }
        return minScorePos;
    }

    return make_tuple(-1,-1,-1);
}
tuple<int,int,int> JoueurIA::alphaBetaPhase2(array<int, 24> sommets, int prof, int alpha, int beta, int id)
{
    if(prof == 0 )
        return make_tuple(heuristique(sommets),-1,-1);

    vector<pair<array<int, 24>,pair<int,int>>> fils;
    fils = generationDesFils2(sommets,id);

    if(id == JOUEUR_IA_ID)
    {
        tuple<int,int,int> nouveau, maxScorePos(-10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            //afficherPlateau((*jt).first);
            //cout << heuristique((*jt).first) << endl;
            nouveau = alphaBetaPhase2((*jt).first,prof-1,alpha,beta,id - 1);
            if(get<0>(maxScorePos) < get<0>(nouveau))
            {
                get<0>(maxScorePos) = get<0>(nouveau);
                get<1>(maxScorePos) = ((*jt).second).first;
                get<2>(maxScorePos) = ((*jt).second).second;
            }
            if(alpha < get<0>(maxScorePos))
                alpha = get<0>(maxScorePos);
            if(beta <= alpha)
                break;
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {


        tuple<int,int,int> nouveau, minScorePos(10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            //afficherPlateau((*jt).first);
            //cout << heuristique((*jt).first) << endl;
            nouveau = alphaBetaPhase2((*jt).first,prof-1,alpha,beta,id + 1);
            if(get<0>(minScorePos) > get<0>(nouveau))
            {
                get<0>(minScorePos) = get<0>(nouveau);
                get<1>(minScorePos) = ((*jt).second).first;
                get<2>(minScorePos) = ((*jt).second).second;
            }
            if(beta > get<0>(minScorePos))
                beta = get<0>(minScorePos);
            if(beta <= alpha)
                break;
        }
        return minScorePos;
    }

    return make_tuple(-1,-1,-1);
}
vector<pair<array<int, 24>,pair<int,int>>> JoueurIA::generationDesFils3(array<int, 24> sommets, int id)
{
    vector<pair<array<int, 24>,pair<int,int>>> fils;
    cout << id <<endl;
    if(id == JOUEUR_IA_ID)
    {
       // pour chaque piece sur le plateau
       for (int i=0;i<24;i++)
       {
           if(sommets[i]==JOUEUR_IA_ID)
           {
               int pos1=i;
               // pour chaque position vide
               for (int j=0; j<24;j++)
               {
                   int pos2 = j;
                   if(sommets[pos2] == 0)
                   {
                       sommets[pos1] = 0;
                       sommets[pos2] = JOUEUR_IA_ID;
                       fils.push_back(make_pair(sommets, make_pair(pos1, pos2)));
                       // on remet le plateau à son état initial
                       sommets[pos1] = JOUEUR_IA_ID;
                       sommets[pos2] = 0;

                   }
                }


            }
        }
    }
    else if(id == JOUEUR_HUMAIN_ID)
    {
       // pour chaque piece sur le plateau
       for(int i=0; i<24;i++)
       {
           if(sommets[i]==JOUEUR_HUMAIN_ID)
           {
               int pos1=i;
                // pour chaque position vide
               for (int j=0; j<24;j++)
               {
                   int pos2 = j;
                   if(sommets[pos2] == 0)
                   {
                       sommets[pos1] = 0;
                       sommets[pos2] = JOUEUR_HUMAIN_ID;
                       fils.push_back(make_pair(sommets, make_pair(pos1, pos2)));
                       // on remet le plateau à son état initial
                       sommets[pos1] = JOUEUR_HUMAIN_ID;
                       sommets[pos2] = 0;
                   }


               }

           }

       }

    }
    return fils;
}
tuple<int,int,int> JoueurIA::miniMaxPhase3(std::array<int, 24> sommets, int prof, int id)
{
    if(prof == 0 )
        return make_tuple(heuristique(sommets),-1,-1);

    vector<pair<array<int, 24>,pair<int,int>>> fils;
    fils = generationDesFils3(sommets,id);

    if(id == JOUEUR_IA_ID)
    {
        tuple<int,int,int> nouveau, maxScorePos(-10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            nouveau = miniMaxPhase3((*jt).first,prof-1,id - 1);
            if(get<0>(maxScorePos) < get<0>(nouveau))
            {
                get<0>(maxScorePos) = get<0>(nouveau);
                get<1>(maxScorePos) = ((*jt).second).first;
                get<2>(maxScorePos) = ((*jt).second).second;
            }
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {


        tuple<int,int,int> nouveau, minScorePos(10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            nouveau = miniMaxPhase3((*jt).first,prof-1,id + 1);
            if(get<0>(minScorePos) > get<0>(nouveau))
            {
                get<0>(minScorePos) = get<0>(nouveau);
                get<1>(minScorePos) = ((*jt).second).first;
                get<2>(minScorePos) = ((*jt).second).second;
            }
        }
        return minScorePos;
    }

    return make_tuple(-1,-1,-1);
}
tuple<int,int,int> JoueurIA::alphaBetaPhase3(array<int, 24> sommets, int prof, int alpha, int beta, int id)
{

    if(prof == 0 )
        return make_tuple(heuristique(sommets),-1,-1);

    vector<pair<array<int, 24>,pair<int,int>>> fils;
    fils = generationDesFils3(sommets,id);

    if(id == JOUEUR_IA_ID)
    {
        tuple<int,int,int> nouveau, maxScorePos(-10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            nouveau = alphaBetaPhase3((*jt).first,prof-1,alpha,beta,id - 1);
            if(get<0>(maxScorePos) < get<0>(nouveau))
            {
                get<0>(maxScorePos) = get<0>(nouveau);
                get<1>(maxScorePos) = ((*jt).second).first;
                get<2>(maxScorePos) = ((*jt).second).second;
            }
            if(alpha < get<0>(maxScorePos))
                alpha = get<0>(maxScorePos);
            if(beta <= alpha)
                break;
        }
        return maxScorePos;

    }


    else if(id == JOUEUR_HUMAIN_ID)
    {


        tuple<int,int,int> nouveau, minScorePos(10000,-1,-1);
        vector<pair<array<int, 24>,pair<int,int>>>::iterator jt;
        for (jt = fils.begin(); jt != fils.end(); jt++)
        {
            nouveau = alphaBetaPhase3((*jt).first,prof-1,alpha,beta,id + 1);
            if(get<0>(minScorePos) > get<0>(nouveau))
            {
                get<0>(minScorePos) = get<0>(nouveau);
                get<1>(minScorePos) = ((*jt).second).first;
                get<2>(minScorePos) = ((*jt).second).second;
            }
            if(beta > get<0>(minScorePos))
                beta = get<0>(minScorePos);
            if(beta <= alpha)
                break;
        }
        return minScorePos;
    }

    return make_tuple(-1,-1,-1);
}

void JoueurIA::reinitialiser(){

    Joueur::reinitialiser();
}
