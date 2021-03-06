/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "about.h"

const char programName[] = "Goro";
const char programVersion[] = "0.1.0";

const char programDescription[] =
  "Go (igo, wei-qi, baduk) engine and cli interface.";

const char programCLIOptions[] =
"  --(white|black) TYPE   player move sources, where player types:\n"
"                    ai   moves generated by computer player\n"
"                   cli   human player using interactive command line interface\n"
"                   gtp   moves read from stdin using go transfer protocol\n"
"  --tictactoe            tictactoe on 3x3 board instead of go\n"
"  --gomoko               five in a row instead of go\n"
"  --board N              set game board for NxN\n"
"  --handicap N number of handicap stones\n"
"  --komi N               number of komi\n"
"  --help                 prints this help";
