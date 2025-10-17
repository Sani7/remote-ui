#!/bin/bash
for arg in "$@"; do
  if [[ "$arg" == "-g" ]]; then
    commit=true
    # Put your command here
  fi
done

if [ "$commit" == true ]; then
    # save unsaved work
    git stash --include-untracked
fi


find . -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' -not -path "./external/*" -not -path "./build/*" -exec clang-format -i {} \;

if [ "$commit" == true ]; then
    git add -A && git commit -m "Lint"
    git stash pop
fi
