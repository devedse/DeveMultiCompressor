#!/bin/sh
set -e

args=""

if [ "$1" = "-T" ] ; then
    args="$1 $2"
    shift
    shift
fi

prog="$1"
file="$2"
if [ '!' '(' "$prog" = "all" -o "$prog" = "preprocess" -o "$prog" = "nncp" -o \
             "$prog" = "trfcp" ')' ] ; then
    echo "Regression tests"
    echo ""
    echo "usage: test.sh [-T n] all|preprocess|nncp|trfcp [infile]"
    exit 1
fi

hash="n"
if [ "$file" = "" ] ; then
    file="test/enwik4"
    hash="y"
fi

if [ $prog = "preprocess" -o $prog = "all" ] ; then

    ./preprocess c /tmp/word.txt "$file" /tmp/out.bin 512 8
    ./preprocess d /tmp/word.txt /tmp/out.bin /tmp/out.txt
    cmp /tmp/out.txt "$file"

fi
    
if [ $prog = "nncp" -o $prog = "all" ] ; then

    ./nncp $args c "$file" /tmp/out-nncp.bin
    if [ $hash = "y" ] ; then
        if [ -f test/out-nncp.hash ] ; then
            sha256sum -c test/out-nncp.hash
        else
            sha256sum /tmp/out-nncp.bin > test/out-nncp.hash
        fi
    fi
    ./nncp $args d /tmp/out-nncp.bin /tmp/out.txt
    cmp /tmp/out.txt "$file"

fi

if [ $prog = "trfcp" -o $prog = "all" ] ; then

    ./trfcp $args c "$file" /tmp/out-trfcp.bin
    if [ $hash = "y" ] ; then
        if [ -f test/out-trfcp.hash ] ; then
            sha256sum -c test/out-trfcp.hash
        else
            sha256sum /tmp/out-trfcp.bin > test/out-trfcp.hash
        fi
    fi
    ./trfcp $args d /tmp/out-trfcp.bin /tmp/out.txt
    cmp /tmp/out.txt "$file"

fi

