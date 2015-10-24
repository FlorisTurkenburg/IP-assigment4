#!/bin/bash

# static files/dirs
cat <<-EOF
    ./web/smarty/*
    *.git*
    *.tar.gz
    *.zip
    *.a
    *.o
    *.sw*
    ./paper/*.pdf
EOF

# object files, classes etc.
find . -type f -exec file \{\} \; | grep -i elf | cut -d: -f1 
find . -type f \( -iname '*.class' -o -iname '*.pyc' \)

# subdirectories
find ./hotel ./paper -mindepth 1 -maxdepth 1 -type d -printf '%p/*\n'

# backup files
find . -type f \( -iname '*~' -o  -iname '*.bak' \)
