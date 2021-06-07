#include "Plateau.h"
#include "ui_Plateau.h"

Plateau::Plateau(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Plateau)
{
    ui->setupUi(this);

    joueurIA = new JoueurIA (moulinPossible, arcs);
    joueurHumain = new JoueurHumain;

    tour = 1;
    phaseJeu[0] = 1;
    phaseJeu[1] = 1;
    moulinDetecte = 0;

    partiesGagnees = 0;
    partiesPerdues = 0;

    QSignalMapper *signalMapper = new QSignalMapper(this);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(pointSelectionne(int)));

    for(int i = 0; i < 24; i++)
    {
        buttons[i] = new QPushButton("", this);

        connect(buttons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(buttons[i], i);

        buttons[i]->setFixedHeight(50);
        buttons[i]->setFixedWidth(50);
        buttons[i]->setObjectName("empty");
    }

    // Définir la feuille de style
    setPlaceHoverStylesheet();

    QGridLayout *boardLayout = new QGridLayout();
    QGridLayout *statusLayout = new QGridLayout();

    // Configurer la disposition des statuts
    // Affichage : message d'état, tour actuel, règles de phase de jeu en cours, bouton de réinitialisation
    QLabel * statsLabel = new QLabel("Stats:");
    statsLabel->setObjectName("boldLabel");
    statusLayout->addWidget(statsLabel, 0, 0, 1, 2);

    turnTextLabel = new QLabel("Current turn:");
    turnTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(turnTextLabel, 1, 0);

    turnLabel = new QLabel(QString::number(tour));
    statusLayout->addWidget(turnLabel, 1, 1);

    gamesWonTextLabel = new QLabel("Games won:");
    gamesWonTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(gamesWonTextLabel, 2, 0);

    gamesWonLabel = new QLabel(QString::number(partiesGagnees));
    statusLayout->addWidget(gamesWonLabel, 2, 1);

    gamesLostTextLabel = new QLabel("Games lost:");
    gamesLostTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(gamesLostTextLabel, 3, 0);

    gamesLostLabel = new QLabel(QString::number(partiesPerdues));
    statusLayout->addWidget(gamesLostLabel, 3, 1);

    statusTextLabel = new QLabel("Status messages:");
    statusTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(statusTextLabel, 4, 0, 1, 2);

    statusList = new QListWidget;
    statusList->setWordWrap(true);
    statusList->addItem("A new game has started.");
    statusLayout->addWidget(statusList, 5, 0, 1, 2);

    QSpacerItem * spacerItem1 = new QSpacerItem(400, 100);
    statusLayout->addItem(spacerItem1, 6, 0, 1, 2);

    gameRulesTextLabel = new QLabel("How to play:");
    gameRulesTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(gameRulesTextLabel, 7, 0, 1, 2);

    gameRulesLabel = new QLabel(reglesJeu[0]);
    gameRulesLabel->setWordWrap(true);
    statusLayout->addWidget(gameRulesLabel, 8, 0, 1, 2);

    QSpacerItem * spacerItem2 = new QSpacerItem(400, 75);
    statusLayout->addItem(spacerItem2, 9, 0, 1, 2);

    resetButton = new QPushButton("Restart game");
    connect(resetButton, SIGNAL(clicked()), SLOT(reinitialiserJeu()));
    statusLayout->addWidget(resetButton, 10, 0, 1, 2);


    // Configurer la disposition du plateau

    boardLayout->setSpacing(0);

    // Étape 1/4: Ajouter les étiquettes des lignes et des colonnes

    for(int i = 7; i > 0; i--) {
      rowLabels[i-1] = new QLabel(QString::number(i));
      rowLabels[i-1]->setStyleSheet("color: grey; text-align: center; padding: 5px;");
      boardLayout->addWidget(rowLabels[i-1], (7-i)*2, 0);
    }

    for(int i = 65; i < 72; i++) {
      char ch = i;
      QString str = QString(ch);
      colLabels[i-65] = new QLabel(str);
      colLabels[i-65]->setAlignment(Qt::AlignCenter);
      colLabels[i-65]->setStyleSheet("color: grey; text-align: center; padding: 5px;");
      boardLayout->addWidget(colLabels[i-65], 13, (i-64)*2-1);
    }

    // Étape 2/4: ajouter les boutons

    boardLayout->addWidget(buttons[0], 0, 1);
    boardLayout->addWidget(buttons[1], 0, 7);
    boardLayout->addWidget(buttons[2], 0, 13);

    boardLayout->addWidget(buttons[3], 2, 3);
    boardLayout->addWidget(buttons[4], 2, 7);
    boardLayout->addWidget(buttons[5], 2, 11);

    boardLayout->addWidget(buttons[6], 4, 5);
    boardLayout->addWidget(buttons[7], 4, 7);
    boardLayout->addWidget(buttons[8], 4, 9);

    boardLayout->addWidget(buttons[9], 6, 1);
    boardLayout->addWidget(buttons[10], 6, 3);
    boardLayout->addWidget(buttons[11], 6, 5);
    boardLayout->addWidget(buttons[12], 6, 9);
    boardLayout->addWidget(buttons[13], 6, 11);
    boardLayout->addWidget(buttons[14], 6, 13);

    boardLayout->addWidget(buttons[15], 8, 5);
    boardLayout->addWidget(buttons[16], 8, 7);
    boardLayout->addWidget(buttons[17], 8, 9);

    boardLayout->addWidget(buttons[18], 10, 3);
    boardLayout->addWidget(buttons[19], 10, 7);
    boardLayout->addWidget(buttons[20], 10, 11);

    boardLayout->addWidget(buttons[21], 12, 1);
    boardLayout->addWidget(buttons[22], 12, 7);
    boardLayout->addWidget(buttons[23], 12, 13);

    // Étape 3/4: Ajouter les lignes verticales

    QLabel *verticalLine [40];

    for(int i = 0; i < 40; i++) {
      verticalLine[i] = new QLabel();
      verticalLine[i]->setPixmap(QPixmap(":/images/vertical.png"));
    }

    boardLayout->addWidget(verticalLine[0],1,1);
    boardLayout->addWidget(verticalLine[1],1,7);
    boardLayout->addWidget(verticalLine[2],1,13);

    boardLayout->addWidget(verticalLine[3],2,1);
    boardLayout->addWidget(verticalLine[4],2,13);

    boardLayout->addWidget(verticalLine[5],3,1);
    boardLayout->addWidget(verticalLine[6],3,3);
    boardLayout->addWidget(verticalLine[7],3,7);
    boardLayout->addWidget(verticalLine[8],3,11);
    boardLayout->addWidget(verticalLine[9],3,13);

    boardLayout->addWidget(verticalLine[10],4,1);
    boardLayout->addWidget(verticalLine[11],4,3);
    boardLayout->addWidget(verticalLine[12],4,11);
    boardLayout->addWidget(verticalLine[13],4,13);

    boardLayout->addWidget(verticalLine[14],5,1);
    boardLayout->addWidget(verticalLine[15],5,3);
    boardLayout->addWidget(verticalLine[16],5,5);
    boardLayout->addWidget(verticalLine[17],5,9);
    boardLayout->addWidget(verticalLine[18],5,11);
    boardLayout->addWidget(verticalLine[19],5,13);

    boardLayout->addWidget(verticalLine[20],7,1);
    boardLayout->addWidget(verticalLine[21],7,3);
    boardLayout->addWidget(verticalLine[22],7,5);
    boardLayout->addWidget(verticalLine[23],7,9);
    boardLayout->addWidget(verticalLine[24],7,11);
    boardLayout->addWidget(verticalLine[25],7,13);

    boardLayout->addWidget(verticalLine[26],8,1);
    boardLayout->addWidget(verticalLine[27],8,3);
    boardLayout->addWidget(verticalLine[28],8,11);
    boardLayout->addWidget(verticalLine[29],8,13);

    boardLayout->addWidget(verticalLine[30],9,1);
    boardLayout->addWidget(verticalLine[31],9,3);
    boardLayout->addWidget(verticalLine[32],9,7);
    boardLayout->addWidget(verticalLine[33],9,11);
    boardLayout->addWidget(verticalLine[34],9,13);

    boardLayout->addWidget(verticalLine[35],10,1);
    boardLayout->addWidget(verticalLine[36],10,13);

    boardLayout->addWidget(verticalLine[37],11,1);
    boardLayout->addWidget(verticalLine[38],11,7);
    boardLayout->addWidget(verticalLine[39],11,13);

    // Étape 4/4: Ajouter les lignes horizontales

    QLabel *horizontalLine [40];

    for(int i = 0; i < 40; i++) {
      horizontalLine[i] = new QLabel();
      horizontalLine[i]->setPixmap(QPixmap(":/images/horizontal.png"));
    }

    boardLayout->addWidget(horizontalLine[0],0,2);
    boardLayout->addWidget(horizontalLine[1],0,3);
    boardLayout->addWidget(horizontalLine[2],0,4);
    boardLayout->addWidget(horizontalLine[3],0,5);
    boardLayout->addWidget(horizontalLine[4],0,6);
    boardLayout->addWidget(horizontalLine[5],0,8);
    boardLayout->addWidget(horizontalLine[6],0,9);
    boardLayout->addWidget(horizontalLine[7],0,10);
    boardLayout->addWidget(horizontalLine[8],0,11);
    boardLayout->addWidget(horizontalLine[9],0,12);

    boardLayout->addWidget(horizontalLine[10],2,4);
    boardLayout->addWidget(horizontalLine[11],2,5);
    boardLayout->addWidget(horizontalLine[12],2,6);
    boardLayout->addWidget(horizontalLine[13],2,8);
    boardLayout->addWidget(horizontalLine[14],2,9);
    boardLayout->addWidget(horizontalLine[15],2,10);

    boardLayout->addWidget(horizontalLine[16],4,6);
    boardLayout->addWidget(horizontalLine[17],4,8);

    boardLayout->addWidget(horizontalLine[18],6,2);
    boardLayout->addWidget(horizontalLine[19],6,4);
    boardLayout->addWidget(horizontalLine[20],6,10);
    boardLayout->addWidget(horizontalLine[21],6,12);

    boardLayout->addWidget(horizontalLine[22],8,6);
    boardLayout->addWidget(horizontalLine[23],8,8);

    boardLayout->addWidget(horizontalLine[24],10,4);
    boardLayout->addWidget(horizontalLine[25],10,5);
    boardLayout->addWidget(horizontalLine[26],10,6);
    boardLayout->addWidget(horizontalLine[27],10,8);
    boardLayout->addWidget(horizontalLine[28],10,9);
    boardLayout->addWidget(horizontalLine[29],10,10);

    boardLayout->addWidget(horizontalLine[30],12,2);
    boardLayout->addWidget(horizontalLine[31],12,3);
    boardLayout->addWidget(horizontalLine[32],12,4);
    boardLayout->addWidget(horizontalLine[33],12,5);
    boardLayout->addWidget(horizontalLine[34],12,6);
    boardLayout->addWidget(horizontalLine[35],12,8);
    boardLayout->addWidget(horizontalLine[36],12,9);
    boardLayout->addWidget(horizontalLine[37],12,10);
    boardLayout->addWidget(horizontalLine[38],12,11);
    boardLayout->addWidget(horizontalLine[39],12,12);

    // Configurer la disposition principale

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QSpacerItem *spacer2 = new QSpacerItem(50,100);

    mainLayout->addLayout(boardLayout);
    mainLayout->addSpacerItem(spacer2);
    mainLayout->addLayout(statusLayout);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);


    setLayout(mainLayout);

    setWindowTitle("Nine Men's Morris");
}

// Setteurs

void Plateau::incTour(){
    tour++;
    updateTurnLabel(tour);
}

void Plateau::incPhaseJeu(){
    updateGameRulesLabel(reglesJeu[phaseJeu[0]]);
    phaseJeu[0]++;
    phaseJeu[1]++;
    updateStatusLabel("Game phase " + QString::number(phaseJeu[0]) + " begins.");
}

void Plateau::incPhaseJeu(Joueur *joueur){
    int j = joueur->getID();
    switch (j) {
    case JOUEUR_HUMAIN_ID:
    {
        phaseJeu[0]++;
        updateGameRulesLabel(reglesJeu[phaseJeu[0]]);
        updateStatusLabel("Game phase " + QString::number(phaseJeu[0]) + " begins for you");
    } break;
    case JOUEUR_IA_ID:
    {
        phaseJeu[1]++;
        updateStatusLabel("Game phase " + QString::number(phaseJeu[1]) + " begins for the computer");
    } break;
    }
}

// Définir des méthodes de feuille de style

void Plateau::setPlaceHoverStylesheet()
{
  // Le joueur sélectionne un point pour placer une pièce

  qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#selected { border: 0px; background-image: url(:/images/blue_stone_selected.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#empty:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); } QPushButton#selected:hover { border: 0px; background-image: url(:/images/blue_stone.png); }");
}

void Plateau::setMoveHoverStylesheet()
{
  // Le joueur sélectionne l'une de ses propres pièces à déplacer

  qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player1:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }");
}

void Plateau::setRemoveHoverStylesheet()
{
  // Le joueur sélectionne l'une des pièces du joueur adverse à retirer
  qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player2:hover { border: 0px; background-image: url(:/images/red_stone_selected.png); }");
}

// Méthodes de mise à jour de l'interface utilisateur

void Plateau::updateTurnLabel(int turn)
{
  turnLabel->setText(QString::number(turn));
}

void Plateau::updateGameRulesLabel(QString str)
{
  gameRulesLabel->setText(str);
}

void Plateau::updateGamesWonLabel(QString str)
{
  gamesWonLabel->setText(str);
}

void Plateau::updateGamesLostLabel(QString str)
{
  gamesLostLabel->setText(str);
}

void Plateau::updateStatusLabel(QString str)
{
  statusList->insertItem(0, str);
  statusList->repaint();
}

void Plateau::showErrorMessage(QString str)
{
  QMessageBox messageBox;
  messageBox.critical(0,"Error",str);
  messageBox.setFixedSize(500,200);
}

// Slots

void Plateau::reinitialiserJeu(){

  for(int i = 0; i < 24; i++)
  {
    buttons[i]->setEnabled(true);
    buttons[i]->setObjectName("empty");
    sommets[i] = 0;
  }

  tour = 1;
  phaseJeu[0] = 1;
  phaseJeu[1] = 1;
  moulinDetecte = 0;
  pointsProteges.clear();

  joueurIA->reinitialiser();
  joueurHumain->reinitialiser();

  statusList->clear();
  statusList->addItem("A new game has started.");

  updateTurnLabel(tour);

  setPlaceHoverStylesheet();

}

void Plateau::pointSelectionne(int pos)
{
    switch(moulinDetecte)
    {
        case 0:
        {
            switch(phaseJeu[0])
            {
              case 1:
                {
                  if(!poserPiece(pos, joueurHumain)) return;

                  if(moulinDetecte == JOUEUR_HUMAIN_ID)
                  {
                    return;
                  }
                } break;
              case 2:
              case 3:
                {
                  static int moveFrom = -1;

                  if(moveFrom == -1)
                  {
                    if(sommets[pos] == JOUEUR_HUMAIN_ID)
                    {
                      moveFrom = pos;
                      buttons[pos]->setObjectName("selected");
                      setPlaceHoverStylesheet();
                    }
                    return;
                  }
                  else if(pos != moveFrom)
                  {
                    switch(phaseJeu[0])
                    {
                    case 2:
                      {
                        if(!deplacerPiece(moveFrom, pos, joueurHumain)) return;
                      } break;
                    case 3:
                      {
                        if(!deplacerPieceLibrement(moveFrom, pos, joueurHumain)) return;
                      } break;
                    }

                    moveFrom = -1;

                    if(moulinDetecte == JOUEUR_HUMAIN_ID)
                    {
                      return;
                    }

                    setMoveHoverStylesheet();
                  }
                  else
                  {
                    moveFrom = -1;

                    buttons[pos]->setObjectName("player"+QString::number(JOUEUR_HUMAIN_ID));

                    setMoveHoverStylesheet();
                    return;
                  }
                } break;
            }
        } break;

        case JOUEUR_HUMAIN_ID:
        {
          if(!enleverPiece(pos, joueurIA)) return;
        } break;
    }

    if((phaseJeu[1] == 3 && joueurIA->getPiecesSurPlateau() < 3) || (phaseJeu[1] == 2 && !aDesMouvementsValides(joueurIA)))
    {
        finDuJeu(joueurIA);
        return;
    }

    // attend 1s
    std::this_thread::sleep_for(std::chrono::seconds(1));
    iaTour();

    // Tour complet
    updateStatusLabel("----\nTurn "+QString::number(tour)+" completed.");
    incTour();

    if((phaseJeu[0] == 1 && phaseJeu[1] == 1 && (joueurHumain->estACourtDePieces() || joueurIA->estACourtDePieces())))
    {
        setMoveHoverStylesheet();

        incPhaseJeu();
    }

    if(phaseJeu[0] == 2 && joueurHumain->getPiecesSurPlateau() <= 3)
    {
        incPhaseJeu(joueurHumain);
    }

    if(phaseJeu[1] == 2 && joueurIA->getPiecesSurPlateau() <= 3)
    {
        incPhaseJeu(joueurIA);
    }

    if((phaseJeu[0] == 3 && joueurHumain->getPiecesSurPlateau() < 3) || (phaseJeu[0] == 2 && !aDesMouvementsValides(joueurHumain)))
    {
        finDuJeu(joueurHumain);
    }
}
//Méthodes IA

void Plateau::iaTour()
{
    switch(phaseJeu[1])
      {
      case 1:
        {
          int aiPos = joueurIA->alphaBetaPhase1(sommets,5,-10000,10000,joueurIA->getID(), joueurIA->getPieces(),joueurHumain->getPieces()).second;

          poserPiece(aiPos, joueurIA);
        } break;
      case 2:
        {
          tuple<int,int,int> iaPos;

          iaPos = joueurIA->alphaBetaPhase2(sommets,5,-10000,10000,JOUEUR_IA_ID);

          deplacerPiece(get<1>(iaPos), get<2>(iaPos), joueurIA);
        } break;
      case 3:
        {
          tuple<int,int,int> iaPos;

          iaPos = joueurIA->alphaBetaPhase3(sommets,5,-10000,10000,JOUEUR_IA_ID);

          deplacerPieceLibrement(get<1>(iaPos), get<2>(iaPos), joueurIA);
        } break;
      }

      if(moulinDetecte == JOUEUR_IA_ID)
      {
        int iaPos = joueurIA->alphaBetaPhase0(sommets,1,-10000,10000,JOUEUR_HUMAIN_ID).second;
        enleverPiece(iaPos, joueurHumain);
      }
}

// Méthodes de tour

bool Plateau::poserPiece(int pos, Joueur *joueur)
{
    if(sommets[pos] != 0)
    {
        showErrorMessage(ERROR_VERTIX_NOT_EMPTY);
        return false;
    }

    if(joueur->estACourtDePieces())
    {
        showErrorMessage(ERROR_OUT_OF_PIECES);
        return false;
    }

    int p = joueur->getID();

    joueur->placePieceSurPlateau(pos);

    switch(p)
    {
        case JOUEUR_HUMAIN_ID:
        {
              buttons[pos]->setObjectName("player1");
              updateStatusLabel("You have placed a piece. You have "+QString::number(joueur->getPieces()) +" piece(s) left.");
              //joueurIA->poserPiece(pos,JOUEUR_HUMAIN_ID);
        } break;
        case JOUEUR_IA_ID:
        {
              buttons[pos]->setObjectName("player2");
              updateStatusLabel(tr("The computer has placed a piece."));
              //joueurIA->poserPiece(pos,JOUEUR_IA_ID);
        } break;
    }

    buttons[pos]->setStyle(qApp->style());
    buttons[pos]->repaint();

    sommets[pos] = joueur->getID();

    detecterMoulin(pos);

    return true;
}

bool Plateau::enleverPiece(int pos, Joueur *joueur)
{
    if(joueur->aDesPiecesUnprotegesSurPlateau(pointsProteges) && std::find(pointsProteges.begin(), pointsProteges.end(), pos) != pointsProteges.end())
    {
         showErrorMessage(ERROR_ILLEGAL_REMOVE);
         return false;
    }

    if(sommets[pos] != joueur->getID())
    {
        showErrorMessage(ERROR_ILLEGAL_REMOVE);
        return false;
    }

    int p = joueur->getID();


    verifieSiMoulinEstCasse(pos, p);

    joueur->enlevePieceDuPlateau(pos);

    sommets[pos] = 0;

    //joueurIA->enleverPiece(pos);

    buttons[pos]->setObjectName("empty");

    switch(p)
    {
        case JOUEUR_HUMAIN_ID:
        {
          updateStatusLabel("The computer has removed a piece.");
        } break;
        case JOUEUR_IA_ID:
        {
          updateStatusLabel("You have removed a piece.");
        } break;
    }

    buttons[pos]->setStyle(qApp->style());
    statusList->repaint();

    if(phaseJeu[0] == 1)
    {
        setPlaceHoverStylesheet();
    }
    else
    {
        setMoveHoverStylesheet();
    }

    moulinDetecte = 0;

    return true;
}

bool Plateau::deplacerPiece(int pos1, int pos2, Joueur *joueur)
{
    bool connectes = sontConnectes(pos1, pos2);
    int p = joueur->getID();

    if(sommets[pos1] != p || sommets[pos2] != 0 || !connectes)
    {
        showErrorMessage(ERROR_ILLEGAL_MOVE);
        return false;
    }

    verifieSiMoulinEstCasse(pos1, p);

    sommets[pos1] = 0;
    sommets[pos2] = p;

    //joueurIA->deplacerPiece(pos1,pos2,joueur->getID());

    joueur->deplacePieceSurPlateau(pos1, pos2);

    buttons[pos1]->setObjectName("empty");
    buttons[pos2]->setObjectName("player"+QString::number(p));
    buttons[pos1]->setStyle(qApp->style());
    buttons[pos2]->setStyle(qApp->style());
    buttons[pos1]->repaint();
    buttons[pos2]->repaint();

    switch(joueur->getID())
    {
        case JOUEUR_IA_ID:
        {
          updateStatusLabel("The computer has moved a piece.");
        } break;
        case JOUEUR_HUMAIN_ID:
        {
          updateStatusLabel("You have moved a piece.");
        } break;
    }

    detecterMoulin(pos2);

    return true;
}

bool Plateau::deplacerPieceLibrement(int pos1, int pos2, Joueur *joueur)
{
    if(sommets[pos1] != joueur->getID() || sommets[pos2] != 0) {
        showErrorMessage(ERROR_ILLEGAL_MOVE);
        return false;
    }

    int p = joueur->getID();

    sommets[pos1] = 0;
    sommets[pos2] = p;

    //joueurIA->deplacerPiece(pos1,pos2,joueur->getID());

    buttons[pos1]->setObjectName("empty");
    buttons[pos2]->setObjectName("player"+QString::number(p));

    joueur->deplacePieceSurPlateau(pos1, pos2);

    buttons[pos1]->setStyle(qApp->style());
    buttons[pos2]->setStyle(qApp->style());
    buttons[pos1]->repaint();
    buttons[pos2]->repaint();

    switch(joueur->getID())
    {
        case JOUEUR_IA_ID:
        {
          updateStatusLabel("The computer has moved a piece.");
        } break;
        case JOUEUR_HUMAIN_ID:
        {
          updateStatusLabel("You have moved a piece.");
        } break;
    }

    detecterMoulin(pos2);

    return true;
}

// Méthodes de logique

bool Plateau::sontConnectes(int pos1, int pos2){
    for(int i = 0; i < 32; i++)
      {
        if(arcs[i][0] == min(pos1, pos2) && arcs[i][1] == max(pos1, pos2))
        {
          return true;
        }
      }
      return false;
}

bool Plateau::aDesMouvementsValides(Joueur *joueur){

    vector<int> piecesSurPlateau = joueur->getPiecesSurPlateauVector();

    for(vector<int>::iterator it = piecesSurPlateau.begin(); it != piecesSurPlateau.end(); it++)
      {
        int pieceNr = *it;

        for(int i = 0; i < 24; i++)
        {
          if(sommets[i] == 0)
          {
            if(sontConnectes(i, pieceNr)) { return true; }
          }
        }
      }

      return false;
}

// Méthode de détection du moulin

void Plateau::detecterMoulin(int pos){

    int p = 0;
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
        pointsProteges.push_back(point1);
        pointsProteges.push_back(point2);
        pointsProteges.push_back(point3);

        p = sommets[point1];
      }
    }
    }
    switch(p)
    {
    case JOUEUR_HUMAIN_ID:
      {
        if(joueurIA->aDesPiecesUnprotegesSurPlateau(pointsProteges) || joueurIA->getPiecesSurPlateau() <= 3)
        {
          updateStatusLabel("You have formed a mill and may remove a piece.");
          setRemoveHoverStylesheet();
        }
        else
        {
          updateStatusLabel("You have formed a mill but all of the computer's pieces are protected. That sucks.");
          return;
        }
      } break;
    case JOUEUR_IA_ID:
      {
        if(joueurHumain->aDesPiecesUnprotegesSurPlateau(pointsProteges) || joueurHumain->getPiecesSurPlateau() <= 3)
        {
          updateStatusLabel("The computer has formed a mill and may remove a piece.");
        }
        else
        {
          updateStatusLabel("The computer has formed a mill but all of your pieces are protected. Phew!");
          return;
        }
      } break;
    }

    moulinDetecte = p;
}

void Plateau::verifieSiMoulinEstCasse(int pos, int p){

    int point1, point2, point3;

    for(int i = 0; i < 16; i++)
    {
    point1 = moulinPossible[i][0];
    point2 = moulinPossible[i][1];
    point3 = moulinPossible[i][2];

    if(point1 == pos || point2 == pos || point3 == pos)
    {
      if(sommets[point1] == p && sommets[point2] == p && sommets[point3] == p)
      {
        pointsProteges.erase(std::find(pointsProteges.begin(), pointsProteges.end(), point1));
        pointsProteges.erase(std::find(pointsProteges.begin(), pointsProteges.end(), point2));
        pointsProteges.erase(std::find(pointsProteges.begin(), pointsProteges.end(), point3));
      }
    }
    }
}

// Méthode de fin du jeu

void Plateau::finDuJeu(Joueur *joueurPerdu){

    switch(joueurPerdu->getID())
      {
      case JOUEUR_IA_ID:
        {
          updateStatusLabel("You have won the game! Congratulations!");
          partiesGagnees++;
          updateGamesWonLabel(QString::number(partiesGagnees));
        } break;
      case JOUEUR_HUMAIN_ID:
        {
          updateStatusLabel("The computer has won the game! Better luck next time.");
          partiesPerdues++;
          updateGamesLostLabel(QString::number(partiesPerdues));
        } break;
      }

      for(int i = 0; i < 24; i++)
      {
        buttons[i]->setEnabled(false);
      }
}
