/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <math.h>

#include "boarditerator.h"
#include "ai.h"

static double searchMaxScore(const MiniMax *self,
                            BoardCoord *coord,
                            Game *game,
                            int lookahead);

const double miniMaxWinScore = INFINITY;
const double miniMaxLoseScore = -INFINITY;

MiniMax createMiniMax(Game *game) {
  MiniMax self;
  self.monteCarloThreshold = 0.0;
  self.lookahead = 3;
  self.game = game;
  return self;
}

static double searchMaxScore(const MiniMax *self,
                            BoardCoord *coord,
                            Game *game,
                            int lookahead)
{
  double result;
  Player winner = game->vtable->winner(game);
  if (   game->vtable->movesPossible(game)
      && (winner == noPlayer)
      && (lookahead > 0))
  {
    double score;
    double maxScore = -INFINITY;
    BoardCoord maxScoredCoord;
    BoardIterator iterator = createBoardIterator(&game->board);
    for(;nextValidGameMove(game, &iterator);) {
      if (self->monteCarloThreshold < ((double)rand()/RAND_MAX)) {
        BoardCoord ignoredCoord;
        Game nextGame = copyGame(game);
        switchGamePlayer(&nextGame);
        setBoardCell(&nextGame.board, iterator.coord, actualGamePlayerCell(game));
        score = - searchMaxScore(self, &ignoredCoord, &nextGame, lookahead-1);
        destructBoard(&nextGame.board);
        if (score > maxScore) {
          maxScore = score;
          maxScoredCoord = iterator.coord;
        }
      }
    }
    *coord = maxScoredCoord;
    result = maxScore;
  } else {
    result = game->vtable->evalPosition(game);
  }
  return result;
}

BoardCoord getMiniMaxMove(MiniMax *self)
{
  BoardCoord coord;
  searchMaxScore(self, &coord, self->game, self->lookahead);
  return(coord);
}
