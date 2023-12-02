#!/bin/bash

function test() {
    PROG=$1
    INPUT=$2
    ANS=$3

    GOT=$(./"$PROG" < "$INPUT" | tail -n 1)
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
