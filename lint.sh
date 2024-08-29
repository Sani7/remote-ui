#!/bin/bash

if (( $# < 1 )); then
    >&2 echo "Illegal number of parameters"
    exit 1
fi

# save unsaved work
git stash --include-untracked

clang-format -i "$@"

git add -A && git commit -m "Lint"

git stash pop
