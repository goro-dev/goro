/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GTP_H__
#define __GTP_H__

#include "player.h"

typedef struct GTP {
  Player player;
  int ID;
  GameMove nextMove;
  int moveRequested;
  GameMove lastSentMove;
} GTP;

extern GTP createGTP(Game *game);
extern GameMove getGTPMove(GTP *self);

#endif
