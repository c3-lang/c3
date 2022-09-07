#!/bin/sh
$IC3 < hello.in > hello.out 2> hello.err
echo $? > hello.ret
diff -u hello.out.expected hello.out &&
    diff -u hello.err.expected hello.err &&
    diff -u hello.ret.expected hello.ret
