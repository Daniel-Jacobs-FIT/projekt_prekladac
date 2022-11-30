#!/usr/bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)
GREEN="\033[0;32m"
IGNOR_CNT=0
IGNORED=""

SCTEST=tests/scanner

cd ${SCTEST} && for DIR in *; do
    cd  ${DIR} && for TEST in *; do
        if [ -f ${TEST} ]; then
            if ! [[ ${TEST} =~ ^@ ]]; then
                echo "${BOLD}${TEST}:${NORMAL}"
                if [ -f refs/${TEST}-ref ]; then
                    diff --color -u refs/${TEST}-ref <(../../../scanner-tests < $TEST)
                    if [ $? == 0 ]; then
                        printf "\U21b3 ${BOLD}${GREEN}SUCCESS${NORMAL}\n\n"
                    fi
                else
                    echo "${BOLD}No Refs Found! Printing STDOUT${NORMAL}"
                    echo "$( ../../../scanner-test < $TEST )"
                fi
            else
                IGNOR_CNT=$((IGNOR_CNT+1))
                IGNORED="${IGNORED}${TEST} "
            fi
        fi
    done
    cd ..
done 
echo "${BOLD}Ignored ${IGNOR_CNT} tests:${NORMAL}"
echo "${BOLD}${IGNORED}${NORMAL}"
