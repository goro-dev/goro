/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "game.h"
#include "minimax.h"
#include "go.h"
#include "boarditerator.h"
#include "group.h"

static int goMoveWorthChecking(Go *self, BoardCoord coord);
static int validGoMove(Go *self, BoardCoord coord);
static int goGameOver(Go *self);
static PlayerId goWinner(Go *self);
static double evalGoPosition(Go *self);
static Go *copyGoGame(Go *self);
static void goMove(Go *self, BoardCoord coord);
static int repeatedGoPosition(Go *self);
static int goMoveDiedInstantly(Go *self, BoardCoord coord);
static void removeDeadGroups(Go *self, BoardCell cell);
static int countGoTerritory(Go *self, BoardCell cell);

static const BoardCoordString standardHandicaps[][9] =
{
  {{"D4"}, {"Q16"}, {""}, },
  {{"D4"}, {"Q16"}, {"D16"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"K10"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}, {"K10"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}, {"K4"}, {"K16"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}, {"K4"}, {"K16"}, {"K10"}},
};

static const GameVirtualTable goVirtualtable = {
  &goMove,
  &validGoMove,
  &goMoveWorthChecking,
  &goGameOver,
  &goWinner,
  &evalGoPosition,
  &copyGoGame
};

void setGoHandicap(Go *self, int handicap) {
  int i;
  assert(handicap >=2 && handicap <= 9);
  for(i = 0; i < handicap; i++) {
    setBoardCell(&self->game.board,
                 stringToBoardCoord(standardHandicaps[handicap-2][i]),
                 gamePlayerCells[firstPlayer]);
  }
  self->game.actualPlayer = secondPlayer;
}

Go createGo(Board board)
{
  Go self;
  self.game = createGame(board);
  self.game.vtable = &goVirtualtable;
  self.history[0] = 0;
  self.history[1] = 0;
  self.komi = 6.5;
  return self;
}

static void goMove(Go *self, BoardCoord coord)
{
  Go *stored = copyGoGame(self);
  if(self->history[1]) destructGame(self->history[1]);
  self->history[1] = self->history[0];
  self->history[0] = stored;
  gameMove(self, coord);
  removeDeadGroups(self, gamePlayerCells[actualGamePlayer(self)]);
  removeDeadGroups(self, gamePlayerCells[otherGamePlayer(self)]);
}

static void removeDeadGroups(Go *self, BoardCell cell) {
  GroupList groupList = createGroupList(&(self->game.board));
  GroupIterator iterator = createGroupIterator(&groupList);
  while(getGroupsByColor(&iterator, cell)) {
    if (groupLiberties(iterator.group) == 0) {
      removeGroup(iterator.group);
    }
  }
  destructGroupList(&groupList);
}

static Go *copyGoGame(Go *self)
{
  Go *copy = copyGame(self);
  copy = (Go *)realloc(copy, sizeof(Go));
  copy->history[0] = self->history[0] ? copyGame(self->history[0]) : 0;
  copy->history[1] = self->history[1] ? copyGame(self->history[1]) : 0;
  copy->komi = self->komi;
  return copy;
}

static int validGoMove(Go *self, BoardCoord coord)
{
  if (getBoardCell(&self->game.board, coord) != emptyBoardCell) {
    return(0);
  } else {
    Go *copy = copyGoGame(self);
    goMove(copy, coord);
    if (goMoveDiedInstantly(copy, coord)) {
      return(0);
    }
    if (repeatedGoPosition(copy)) {
      return(0);
    }
    destructGame(copy);
  }
  return(1);
}

static int goMoveDiedInstantly(Go *self, BoardCoord coord) {
  return getBoardCell(&self->game.board, coord) == emptyBoardCell;
}

static int repeatedGoPosition(Go *self) {
  return    self->history[1]
         && boardEqual(&self->game.board,
                       &self->history[1]->board);
}

static int goMoveWorthChecking(Go *self, BoardCoord coord)
{
  return 1;
}

static int goGameOver(Go *self)
{
  int gameOver = 1;
  BoardIterator iterator = createBoardIterator(&(self->game.board));
  while(!boardIteratorFinished(&iterator)) {
    if(validGoMove(self, iterator.coord)) {
      gameOver = 0;
    }
  }
  return gameOver;
}

static int countGoTerritory(Go *self, BoardCell cell) {
  int territory = 0;
  GroupList groupLists = createGroupList(&(self->game.board));
  GroupIterator iterator = createGroupIterator(&groupLists);
  while(getTerritories(&iterator)) {
    if (territoryOwner(iterator.group) == cell) {
      territory += iterator.group->size;
    }
  }
  destructGroupList(&groupLists);
  return 0;
}

static PlayerId goWinner(Go *self)
{
  if(goGameOver(self)) {
    return (  countGoTerritory(self, gamePlayerCells[firstPlayer])
            > countGoTerritory(self, gamePlayerCells[secondPlayer])
              + self->komi )
           ? firstPlayer : secondPlayer;
  } else {
    return(noPlayer);
  }
}

static double evalGoPosition(Go *self)
{
  PlayerId winner = goWinner(self);
  if (winner == actualGamePlayer(self)) {
    return miniMaxWinScore;
  } else if (winner == otherGamePlayer(self)) {
    return miniMaxLoseScore;
  } else {
    return(0);
  }
}

