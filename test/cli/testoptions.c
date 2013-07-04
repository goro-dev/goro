/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "options.h"

static char* argv[7] = {
  "bin/goro",
  "--option1",
  "--option2",
  "--option3",
  "option4",
  "--option5",
  "option6",
};

static CliOptionParser options;

static void optionSizeIsOneLessThanArgumentList(void) {
  assert(options.size == 6);
}

static void boundaryIndexedOptionsFound(void) {
  assert(cliOptionSet(&options, "--option1"));
  assert(cliOptionSet(&options, "option6"));
}

static void queryForNotSetOptionResultsFalseWithEmptyValue(void)
{
  assert(!cliOptionSet(&options, "--option7"));
  assert(strcmp(getCliOptionValue(&options, "--options7"),"") == 0);
}

static void optionValueFoundForLastOption(void)
{
  assert(strcmp(getCliOptionValue(&options, "--option5"),"option6") == 0);
}

void testoptions(void)
{
  options = createCliOptionParser(sizeof(argv)/sizeof(char*), argv);
  optionSizeIsOneLessThanArgumentList();
  boundaryIndexedOptionsFound();
  queryForNotSetOptionResultsFalseWithEmptyValue();
  optionValueFoundForLastOption();
}
