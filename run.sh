#!/bin/bash

function check() {
    PROG=$1
    INPUT=$2
    OUTPUT_FILE=$3

    #echo $ANS

    GOT=$(./"$PROG" < "$INPUT" | tail -n 1)
    if [ $? -ne 0 ]; then
        echo -e "[\033[91mFAIL\033[0m] '$PROG < $INPUT | tail -n 1' non zero exit code"
        exit
    fi
    if [ ! -f $OUTPUT_FILE ]; then
        echo -e "[\033[0;34mRUN\033[0m] '$PROG < $INPUT | tail -n 1' = \033[1;37m$GOT\033[0m"
        return 1
    fi

    ANS=$(tail -n 1 $OUTPUT_FILE)
    if [ -z "$(diff <(echo $ANS) <(echo $GOT))"  ]; then
        echo -e "[\033[0;32mPASS\033[0m] '$PROG < $INPUT' = \033[1;37m$ANS\033[0m"
    else
        echo -e "[\033[91mFAIL\033[0m] '$PROG < $INPUT | tail -n 1' != $ANS. Got $GOT"
    fi
}

function check_dir() {
    PROG=$1
    INPUTS=$(find $(dirname $PROG) -name '*input')
    PART="${PROG: -1}" # last character in PROG should be 1 or 2
    for INPUT in $INPUTS ; do
        if [ -f "$PROG" ]; then
            check "$PROG" $INPUT "$INPUT.ans.$PART"
        fi
    done
}

# Export functions to get them to work in find
#export -f check check_dir
#find 2023 -type d -name 'day*' -exec bash -c 'check_dir "$0"' {} \;

# Check for input files for each program passed as arguments and run them
for PROG in "$@" ; do
    check_dir $PROG
done
