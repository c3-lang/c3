#!/bin/sh
{
    echo "\"Hello, world !\"" |
        $IC3 > hello.out 2> hello.err
} &&
    diff -u hello.out.expected hello.out &&
    diff -u hello.err.expected hello.err
