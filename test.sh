#!/bin/bash

IN=0
ERR=0
ALL=0
PASS=0

if [ $# == 2 ]; then
    PROG=$1
    DIR=$2
else
    echo "WRONG NUMBER OF ARGUMENTS"
    exit 1
fi

if [ -f ${PROG} ] && [ -d ${DIR} ]; then
    PROG="./$PROG"
else
    echo "BAD PARAMETERS"
    exit 1
fi

for test in ${DIR}/*.in; do
    TEST_NAME=${test##${DIR}/}
    ${PROG} ${PARAM-} <${test} >output 2>errs
    echo "${TEST_NAME%.in}.out: "
    if diff output "${test%.in}.out" >/dev/null 2>&1; then
        echo "PASS"
        IN=1
    else
        echo "FAIL"
    fi
    echo "${TEST_NAME%.in}.err: "
    if diff errs "${test%.in}.err" >/dev/null 2>&1; then
        echo "PASS"
        ERR=1
    else
        echo "FAIL"
    fi
    if [ "$IN" -eq "1" ] && [ "$ERR" -eq "1" ]; then
        PASS=$((PASS+1))
    fi
    echo ""
    ALL=$((ALL+1))
done

echo "TESTS PASSED: ${PASS}/${ALL}"   

rm -f output
rm -f errs

exit 0

