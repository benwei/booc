## About this repository [![Build Status](https://travis-ci.org/benwei/booc.svg?branch=master)](https://travis-ci.org/benwei/booc)
This is experimental projects which dervied from ooc (object oriented C).
I used the concepts for ooc to create tools such as calculator run on terminal console

## bcalc
the calculator is a intermidated tool created by parsing experssion

### Syntax
syntax: [-v]
 -v: show debug logs

### tests

```
$ make tests
sh ./mytests.sh "bcalc -v"
OK, origin= 2 * 2 + 10 * 2 + 4 / 8 + 1, result=25.5
OK, origin= (2 * 2) + 10 * (2 + 4) / 8 + 1, result=12.5
OK, origin= (2 * -2) + 10 * (2 + 4) / 8 - 1, result=2.5
```
### run sample

```
cat tests/file01.txt | ./bcalc
bcalc version 0.1.1000
0.5
20
990
```

## Reference
* [ooc pdf](http://www.cs.rit.edu/~ats/books/ooc.pdf)
* [sample code of parse.c](http://www.cs.rit.edu/~ats/oop-2001-2/code/13/parse.c)
* [ooc coding site](http://ooc-coding.sourceforge.net/)

## changes
* 2016-08-12 support travis-ci
