/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "boardcoord.h"

const BoardSize standardBoardSizes[standardBoardTypes] =
{
  3, 5, 9, 13, 17, 19
};

const BoardCoord nullBoardCoord = { -1, -1 };

static const char boardColumnCharacters[] = "ABCDEFGHJKLMNOPQRST";

static const BoardCoord fourNeighbours[] = {
              { -1, 0 },
  {  0, -1 },            {  0, 1 },
              {  1, 0 },
};

static const BoardCoord eightNeighbours[] = {
  { -1, -1 }, { -1, 0 }, { -1, 1 },
  {  0, -1 },            {  0, 1 },
  {  1, -1 }, {  1, 0 }, {  1, 1 }
};

const Neighbourhood fourNeighbourhood = { 0, 3, fourNeighbours };
const Neighbourhood eightNeighbourhood = { 0, 7, eightNeighbours };

char boardColumnToChar(BoardSize column) {
  if (   column >= 0
      && column < sizeof(boardColumnCharacters)-1)
  {
    return boardColumnCharacters[column];
  } else {
    fprintf(stderr, "Invalid board column: %d\n", column);
    return '?';
  }
}

BoardSize charToBoardColumn(char character) {
  BoardSize column = 0;
  while(boardColumnCharacters[column] != toupper(character)) {
    if (column >= sizeof(boardColumnCharacters)) {
      fprintf(stderr, "Invalid board column character: %c\n", character);
      return -1;
    }
    column++;
  }
  return column;
}

BoardCoord createBoardCoord(BoardSize row, BoardSize col) {
  BoardCoord self;
  self.row = row;
  self.col = col;
  return self;
}

BoardCoord mulBoardCoord(int multiplicator, BoardCoord self) {
  BoardCoord coord;
  coord.row = multiplicator * self.row;
  coord.col = multiplicator * self.col;
  return coord;
}

BoardCoord subBoardCoords(BoardCoord self, BoardCoord sub) {
  BoardCoord coord = self;
  coord.row -= sub.row;
  coord.col -= sub.col;
  return coord;
}

BoardCoord addBoardCoords(BoardCoord self, BoardCoord add) {
  BoardCoord coord = self;
  coord.row += add.row;
  coord.col += add.col;
  return coord;
}

BoardCoordString createBoardCoordString(char *string) {
  BoardCoordString self;
  snprintf(self.chars, sizeof(self.chars), "%s", string);
  return self;
}

BoardCoord stringToBoardCoord(BoardCoordString string)
{
  BoardCoord coord;
  char character;
  int number;
  int partsFound = sscanf(string.chars,
                          "%[abcdefghjklmnopqrstABCDEFGHJKLMNOPQRST]%d",
                          &character, &number);
  coord.col = charToBoardColumn(toupper(character));
  coord.row = number-1;
  if(partsFound != 2 || number == -1) {
    return nullBoardCoord;
  } else {
    return coord;
  }
}

BoardCoordString boardCoordToString(BoardCoord coord)
{
  BoardCoordString string;
  int charsWritten = (  (boardCoordsEqual(coord, nullBoardCoord))
                      ||(coord.row < 0 || coord.col < 0) )
      ? snprintf(string.chars, sizeof(string.chars), "nan")
      : snprintf(string.chars, sizeof(string.chars), "%c%d",
                 boardColumnCharacters[coord.col], coord.row+1);
  assert(charsWritten < sizeof(string.chars));
  string.chars[sizeof(string.chars)-1] = '\0';
  return string;
}

int boardCoordsEqual(BoardCoord self, BoardCoord reference) {
  return    (self.row == reference.row)
         && (self.col == reference.col);
}

Neighbourhood halfNeighbourhood(Neighbourhood *self)
{
  Neighbourhood neighbourhood = *self;
  neighbourhood.endIndex = self->endIndex / 2;
  return neighbourhood;
}
