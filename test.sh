#!/bin/bash

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
    echo "${TEST_NAME%.in}: "
    ${PROG} ${PARAM-} <${test} >output 2>errs
    if diff output "${test%.in}.out" >/dev/null 2>&1; then
        echo "PASS"
        PASS=$((PASS+1))
    else
        echo "FAIL"
    fi

    echo ""
    ALL=$((ALL+1))
done

echo "TESTS PASSED: ${PASS}/${ALL}"   

rm -f output
rm -f errs

exit 0

