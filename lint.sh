#!/bin/bash
# save unsaved work
git stash --include-untracked

find . -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' -not -path "./external/*" -not -path "./build/*" -exec clang-format -i {} \;

git add -A && git commit -m "Lint"

git stash pop
