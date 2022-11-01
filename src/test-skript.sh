#!/usr/bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

cd tests/ && for TEST in *; do
    echo -n "${BOLD}${TEST}${NORMAL}"
    diff --color -u <(tail -n +3 $TEST) <(../scanner-test < <(head -n 1 $TEST))
    if [ $? == 0 ]; then
        echo "${BOLD} - SUCCESS${NORMAL}"
    fi
done
    
