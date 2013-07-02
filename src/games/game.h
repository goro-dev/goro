/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include "boarditerator.h"

typedef enum {
  noPlayer = -1,
  firstPlayer = 0,
  secondPlayer = 1,
  numberOfPlayers = 2
} PlayerId;

typedef enum {
  playMove,
  passMove,
  resignMove,
} GameMoveType;

typedef struct GameMove {
  BoardCoord coord;
  GameMoveType type;
} GameMove;

typedef struct Game Game;

typedef struct GameVirtualTable {
  void (*move)(Game *self, BoardCoord coord);
  int (*validMove)(Game *self, BoardCoord coord);
  int (*moveWorthChecking)(Game *self, BoardCoord coord);
  int (*over)(Game *self);
  PlayerId (*winner)(Game *self);
  double (*evalPosition)(Game *self);
  Game *(*copy)(Game *self);
  double (*score)(Game *self);
  void (*printGameStatus)(Game *self);
} GameVirtualTable;

struct Game {
  PlayerId actualPlayer;
  Board board;
  BoardCoord lastMove;
  const GameVirtualTable *vtable;
};

extern const BoardCell gamePlayerCells[numberOfPlayers];
extern const char *gamePlayerNames[numberOfPlayers];
extern Game createGame(Board board);
extern Game *copyGame(Game *self);
extern void destructGame(Game *self);
extern void gameMove(Game *self, BoardCoord coord);
extern BoardCell actualGamePlayerCell(Game *self);
extern int nextValidGameMove(Game *self, BoardIterator *iterator);
extern int nextMoveWorthChecking(Game *self, BoardIterator *iterator);
extern PlayerId otherGamePlayer(Game *self);
extern PlayerId actualGamePlayer(Game *self);
extern void printGameOverInfo(Game *self);
extern void printGameStatus(Game *self);

#endif