#!/bin/bash
NOTES_ROOT=$(dirname $(readlink -f $0))
EXTENSIONS="*.md"
FORMAT="markdown"
NOTEBOOK="shooty-face"

echo
echo "#####"
echo "Syncing <$EXTENSIONS> from <$NOTES_ROOT> to notebook <$NOTEBOOK>"
echo "Formatting with <$FORMAT>"
echo "#####"
printf "..."

set -e
gnsync -p $NOTES_ROOT  -m $EXTENSIONS -f $FORMAT -n $NOTEBOOK

if [[ $? == 0 ]]; then
    echo "Done"
else
    echo "Failed"
fi
