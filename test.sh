#!/bin/bash

function test() {
    PROG=$1
    INPUT=$2
    ANS=$3

    GOT=$(./"$PROG" < "$INPUT" | tail -n 1)
    if [ $? -ne 0 ]; then
        echo -e "[\033[91mFAIL\033[0m] '$PROG < $INPUT | tail -n 1' non zero exit code"
        exit
    fi
    if [ $GOT -ne $ANS ]; then
        echo -e "[\033[91mFAIL\033[0m] '$PROG < $INPUT | tail -n 1' != $ANS. Got $GOT"
    else
        echo -e "[\033[0;32mPASS\033[0m] '$PROG < $INPUT' = \033[1;37m$GOT\033[0m"
    fi
}

test d2p1 d2_test 8
test d2p1 d2_input 2810
test d2p2 d2_test 2286
test d2p2 d2_input 69110
test d3p1 d3_test 4361
test d3p1 d3_input 540131
test d3p2 d3_test 467835
test d4p1 d4_test 13
test d4p1 d4_input 24542
test d4p2 d4_test 30
test d4p2 d4_input 8736438
