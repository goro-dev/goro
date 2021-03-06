/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <assert.h>

#include "gamemove.h"

static void createdGameMoveGetsCoordAndType(void) {
  GameMove move = createGameMove(createBoardCoord(4,6), playMove);
  assert(boardCoordsEqual(move.coord, createBoardCoord(4,6)));
  assert(move.type == playMove);
}

static void everyMoveTypeGetAppropriateType(void) {
  assert(createResignMove().type == resignMove);
  assert(createPassMove().type == passMove);
  assert(createPlayMove(nullBoardCoord).type == playMove);
}

static void playMovesEqualOnlyIfCoordsEqual(void) {
  assert(gameMoveEqual(createPlayMove(createBoardCoord(4,6)),
                       createPlayMove(createBoardCoord(4,6))));
  assert(!gameMoveEqual(createPlayMove(createBoardCoord(4,5)),
                        createPlayMove(createBoardCoord(4,6))));
}

static void nonPlayMovesEqualEvenIfCoordsDiffer(void) {
  assert(gameMoveEqual(createGameMove(createBoardCoord(4,6), passMove),
                       createGameMove(createBoardCoord(4,5), passMove)));
  assert(gameMoveEqual(createGameMove(createBoardCoord(4,6), resignMove),
                       createGameMove(createBoardCoord(4,5), resignMove)));
}

static void movesDifferIfAtLeastTypesDiffer(void) {
  assert(!gameMoveEqual(createGameMove(createBoardCoord(4,6), resignMove),
                        createGameMove(createBoardCoord(4,6), playMove)));
  assert(!gameMoveEqual(createGameMove(createBoardCoord(4,6), resignMove),
                        createGameMove(createBoardCoord(4,6), passMove)));
  assert(!gameMoveEqual(createGameMove(createBoardCoord(4,6), playMove),
                        createGameMove(createBoardCoord(4,6), passMove)));
}

static void playMoveConvertToStringOfItsCoord(void) {
  GameMoveString playString = gameMoveToString(createPlayMove(createBoardCoord(4,6)));
  assert(strcmp(playString.chars, "G5") == 0);
}

static void specialMovesConvertToStringOfTheirType(void) {
  GameMoveString passString = gameMoveToString(createPassMove());
  GameMoveString resignString = gameMoveToString(createResignMove());
  assert(strcmp(passString.chars, "pass") == 0);
  assert(strcmp(resignString.chars, "res") == 0);
}

static void testGameMoveStringType(char *string, GameMoveType type) {
  assert(stringToGameMove(createGameMoveString(string)).type == type);
}

static void specialMoveStringsConvertsToSpecialMoves(void) {
  testGameMoveStringType("pass", passMove);
  testGameMoveStringType("res", resignMove);
}

static void invalidStringsConvertToInvalidMove(void) {
  testGameMoveStringType("notamove", invalidMove);
}

static void coordStringConvertsToCorrespondingPlayMove(void) {
  assert(gameMoveEqual(stringToGameMove(createGameMoveString("G5")),
                       createPlayMove(createBoardCoord(4,6))));
}

static MoveIterator exampleMoveIterator(void) {
  Board *board = malloc(sizeof(Board));
  *board = createBoard(13,19);
  return createMoveIterator(board);
}

static MoveIterator moveIteratorStepped(int steps) {
  int i;
  MoveIterator iterator = exampleMoveIterator();
  for(i = 0; i < steps; i++) moveIteratorFinished(&iterator);
  return iterator;
}

static void testIteratorMove(MoveIterator iterator, GameMove move) {
  assert(gameMoveEqual(iterator.move, move));
}

static void notStartedMoveIteratorGivesNullMove(void) {
  testIteratorMove(moveIteratorStepped(0), nullMove);
}

static void moveIteratorGivesPassAtFirstStep(void) {
  testIteratorMove(moveIteratorStepped(1), createPassMove());
}

static void moveIteratorGivesResignAtSecondStep(void) {
  testIteratorMove(moveIteratorStepped(2), createResignMove());
}

static void moveIteratorGivesTopLeftPlayAtThirdStep(void) {
  testIteratorMove(moveIteratorStepped(3),
                   createPlayMove(createBoardCoord(0,0)));
}

static void moveIteratorGivesEveryBoardCellOnce(void)
{

}

static void notFinishingMoveIteratorResultsTrue(void)
{
  int i;
  for(i = 0; i < 2 + 13*19; i++) {
    MoveIterator iterator = moveIteratorStepped(i);
    assert(!moveIteratorFinished(&iterator));
  }
}

static void finishingMoveIteratorGivesBottomRightPlay(void)
{
  testIteratorMove(moveIteratorStepped(2 + 13*19),
                   createPlayMove(createBoardCoord(18,12)));
}

static void finishingMoveIteratorResultsFalse(void)
{
  MoveIterator iterator = moveIteratorStepped(2 + 13*19);
  assert(moveIteratorFinished(&iterator));
}

static void finishedMoveIteratorGivesNullMove(void)
{
  testIteratorMove(moveIteratorStepped(2 + 13*19 + 1), nullMove);
}

static void testGameMove(void) {
  createdGameMoveGetsCoordAndType();
  everyMoveTypeGetAppropriateType();
  playMovesEqualOnlyIfCoordsEqual();
  nonPlayMovesEqualEvenIfCoordsDiffer();
  movesDifferIfAtLeastTypesDiffer();
}

static void testGameMoveString(void) {
  playMoveConvertToStringOfItsCoord();
  specialMovesConvertToStringOfTheirType();
  specialMoveStringsConvertsToSpecialMoves();
  invalidStringsConvertToInvalidMove();
  coordStringConvertsToCorrespondingPlayMove();
}

static void testGameMoveIterator(void) {
  notStartedMoveIteratorGivesNullMove();
  notFinishingMoveIteratorResultsTrue();
  moveIteratorGivesPassAtFirstStep();
  moveIteratorGivesResignAtSecondStep();
  moveIteratorGivesTopLeftPlayAtThirdStep();
  moveIteratorGivesEveryBoardCellOnce();
  finishingMoveIteratorGivesBottomRightPlay();
  finishingMoveIteratorResultsFalse();
  finishedMoveIteratorGivesNullMove();
}

void testgamemove(void)
{
  testGameMove();
  testGameMoveString();
  testGameMoveIterator();
}
