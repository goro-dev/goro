/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "board.h"
#include "boardcoord.h"
#include "boarditerator.h"

const char boardSigns[] = { '.', '@', 'O'};

int boardCellFilled(BoardCell self)
{
  return    self != emptyBoardCell
         && self != invalidBoardCell;
}

Board createBoard(BoardSize width,
                  BoardSize height)
{
  Board self;
  self.width  = width;
  self.height = height;
  self.cells  = malloc(width * height * sizeof(self.cells[0]));
  assert(self.cells != 0);
  clearBoard(&self);
  return self;
}

void clearBoard(Board *self)
{
  memset(self->cells, emptyBoardCell,
         self->width * self->height * sizeof(self->cells[0]));
}

Board copyBoard(Board *self)
{
  Board board = createBoard(self->width, self->height);
  memcpy(board.cells, self->cells,
         self->width * self->height * sizeof(self->cells[0]));
  return(board);
}

void destructBoard(Board *self)
{
  free(self->cells);
  self->cells = 0;
}

int coordInBoard(Board *self, BoardCoord coord)
{
  return (   coord.row >= 0 && coord.row < self->height
          && coord.col >= 0 && coord.col < self->width);
}

int boardCellHasNeighbour(Board *self, BoardCoord coord,
                          Neighbourhood neighbourhood)
{
  int hasNeighbour = 0;
  NeighbourIterator iterator =
      createNeighbourIterator(coord, neighbourhood);
  while(getNeighbours(&iterator))
  {
    BoardCell neighbour = getBoardCell(self, iterator.neighbour);
    hasNeighbour |= boardCellFilled(neighbour);
  }
  return hasNeighbour;
}

BoardCell getBoardCell(Board *self, BoardCoord coord)
{
  if(coordInBoard(self, coord))
    return self->cells[coord.row*self->width + coord.col];
  return invalidBoardCell;
}

void setBoardCell(Board *self, BoardCoord coord, BoardCell cell)
{
  assert(coordInBoard(self, coord));
  self->cells[coord.row * self->width + coord.col] = cell;
}

void clearBoardCell(Board *self, BoardCoord coord)
{
  assert(coordInBoard(self, coord));
  self->cells[coord.row * self->width + coord.col] = emptyBoardCell;
}

void printBoard(Board *self)
{
  BoardIterator iterator = createBoardIterator(self);
  int i;
  printf("   ");
  for(i = 0; i < self->width; i++) printf("%c ", boardColumnToChar(i));
  printf("\n");
  while(!boardIteratorFinished(&iterator))
  {
    if (iterator.coord.col == 0) printf("%2d ", iterator.coord.row+1);
    printf("%c ", boardSigns[getBoardCell(self, iterator.coord)]);
    if (iterator.coord.col == self->width-1) printf("\n");
  }
}

int boardHasEmptyCell(Board *self)
{
  int emptyCellCount = 0;
  BoardIterator iterator = createBoardIterator(self);
  while(!boardIteratorFinished(&iterator)) {
    if (getBoardCell(self, iterator.coord) == emptyBoardCell) {
      emptyCellCount++;
    }
  }
  return(emptyCellCount > 0);
}

int boardEqual(Board *self, Board *ref)
{
  if (   (self->height != ref->height)
      || (self->width != ref->width))
  {
    return 0;
  } else {
    int equal = 1;
    BoardIterator iterator = createBoardIterator(self);
    while(!boardIteratorFinished(&iterator)) {
      if (getBoardCell(self, iterator.coord) != getBoardCell(ref, iterator.coord))
      {
        equal = 0;
      }
    }
    return equal;
  }
}

void saveBoard(Board *self, FILE *file)
{
  BoardSize width  = self->width;
  BoardSize height = self->height;
  if(    ( fwrite(&width,       sizeof(int), 1, file) == 1 )
      && ( fwrite(&height,      sizeof(int), 1, file) == 1 )
      && ( fwrite(self->cells, sizeof(self->cells[0]),
                  width*height, file) == width*height) )
  {
    return;
  }
  else
  {
    fprintf(stderr,"Can not save board file.");
    return;
  }
}

Board loadBoard(FILE *file)
{
  BoardSize width, height;
  Board self;
  if (   (fread(&width,  sizeof(int), 1, file) == 1)
      && (fread(&height, sizeof(int), 1, file) == 1) )
  {
    self = createBoard(width, height);
    if (   fread(self.cells, sizeof(self.cells[0]), width*height, file)
        == width*height)
    {
      return(self);
    }
    destructBoard(&self);
  }
  fprintf(stderr,"Can not load board file.");
  assert(0);
  return(self);
}
