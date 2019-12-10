#!/usr/bin/env bash

########################################################
# This file was imported from
# https://github.com/srajangarg/symengine/blob/master/bin/travis_clang.sh
#
# Modified by Michael Wurm <wurm.michael95@gmail.com>
########################################################

RETURN=0
CLANG_FORMAT="clang-format-6.0"

if [ ! -f ".clang-format" ]; then
  echo ".clang-format file not found!"
  exit 1
fi

FILES=`git ls-files | grep -E "\.(cpp|h)$" | grep -Ev "lib/"`

for FILE in $FILES; do
  echo "Processing: $FILE"

  $CLANG_FORMAT $FILE | cmp $FILE >/dev/null

  if [ $? -ne 0 ]; then
    echo "[!] INCORRECT FORMATTING! $FILE" >&2
    $CLANG_FORMAT -i $FILE
    RETURN=1
  fi
done

if [ $RETURN -ne 0 ]; then
  RED='\033[0;31m'
  echo -e "\\n${RED}FORMATTING TEST FAILED\\n"
  echo "Apply the following diff for correct formatting:"
  echo "###########################################################################"
  git diff | cat
  echo "###########################################################################"
else
  GREEN='\033[0;32m'
  echo -e "\\n${GREEN}FORMATTING TEST PASSED\\n"
fi

exit $RETURN
