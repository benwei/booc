#!/bin/sh



check_result()
{
    ret=$(eval 'echo "$myexp" | ./ooc_calc | strings | grep "process result =" | sed -e "s/^.*=[ ]*\([0-9.]*\)/\1/g"')

    compare="$1"
    if [ "$ret" = "$compare" ]; then
        echo "OK, origin= $myexp, result=$ret"
    else
        echo "failed, origin= $myexp, result=$ret"
    fi
}

myexp="2 * 2 + 10 * 2 + 4 / 8 + 1"
check_result "25.5"

myexp="(2 * 2) + 10 * (2 + 4) / 8 + 1"
check_result "12.5"

myexp="(2 * -2) + 10 * (2 + 4) / 8 - 1"
check_result "2.5"
