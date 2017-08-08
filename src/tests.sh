#!/bin/sh
# tests cases written by ben6 2014-05

target="$1"
if [ -z "$target" ]; then
target="bcalc"
fi

docalc() {
    ret=`echo "${myexp}" | ./${target}`
    eval "$1=$ret"
}

docalc_from_file() {
    ret=`cat "$1" | ./${target}`
    echo "$ret"
}

show_result ()
{
    ret="$1"
    compare="$2"
    exp="$3"
    if [ "$ret" = "${compare}" ]; then
        echo "OK, origin=$exp, result=$ret"
    else
        echo "failed, origin=$exp, result=$ret"
    fi
}

check_multiple_result_from_file()
{
    counter=1
    test_file="$1"
    shift
    docalc_from_file "$test_file" | while read r ; do
        eval "expdisp=\"`cat $test_file | head -n $counter | tail -n 1`\""
        show_result "$r" "$1" "$expdisp"
        counter=$((counter + 1))
        shift
    done
}

check_single_result()
{
    docalc result
    show_result "$result" "$1" "$myexp"
}

myexp="2 * 2 + 10 * 2 + 4 / 8 + 1"
check_single_result "25.5"

myexp="(2 * 2) + 10 * (2 + 4) / 8 + 1"
check_single_result "12.5"

myexp="(2 * -2) + 10 * (2 + 4) / 8 - 1"
check_single_result "2.5"

check_multiple_result_from_file "tests/file01.txt" "0.5" "20" "990" "12.6667"
