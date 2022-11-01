#!/usr/bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

cd tests-in/ && for TEST in *; do
    echo "${BOLD}${TEST}:${NORMAL}"
    diff --color -u ../tests-ref/${TEST}-ref <(../scanner-test < $TEST)
    if [ $? == 0 ]; then
        printf "\U21b3 ${BOLD}SUCCESS${NORMAL}\n\n"
    fi
done
    
