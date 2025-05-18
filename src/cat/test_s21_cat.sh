#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""
FLAGS="b e n s t v"
RED="$(tput setaf 1)"
GREEN="$(tput setaf 2)"
BASE="$(tput setaf 7)"
declare -a tests=(
    "VAR test_case_cat.txt"
)

testing() {
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    RESULT="SUCCESS"
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        RESULT="SUCCESS"
    else
      (( FAIL++ ))
        RESULT="FAIL"
    fi
    echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"
    rm test_s21_cat.log test_sys_cat.log
}


# 1 параметр
for var1 in $FLAGS
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

echo "${RED}FAIL: ${FAIL}${BASE}"
echo "${GREEN}SUCCESS: ${SUCCESS}${BASE}"