#/bin/bash

app_name=app.exe
# build 
make $app_name
mv $app_name func_tests

cd func_tests/
# creating out buffer
touch out.txt

# testing

declare -i total_tests=0
declare -i passed_tests=0
declare -i failed_tests=0

# testcase <exp-result-code> <exp-digit> <str-input>
function postest() {
    argc=$(cat $3)
    ./$app_name $argc < $2 > out.txt
    status=$?

    if [ -z "$(diff -q $3 out.txt)" ]
    then
        passed_tests=$((passed_tests + 1))
    else
        printf "Failed for $3| return code is $status\n"
        printf "$(diff -c $3 out.txt)\n"
        printf "\n====\n"
        printf "Expected: \n"
        cat $3
        printf "\n====\n"
        printf "Got: \n"
        cat out.txt
        printf "\n====\n"
        failed_tests=$((failed_tests + 1))
        printf "<---------------------------------->\n"
    fi
    
    total_tests=$((total_tests + 1))
}

function negtest() {
    argc=$(cat $1)
    ./$app_name $argc < $2 > out.txt
    status=$?
    if [[ $status == 0 ]]
    then
        printf "Failed for $3| return code is $status\n"
        printf "$(diff -c $3 out.txt)\n"
        printf "\n====\n"
        printf "Expected: \n"
        cat $3
        printf "\n====\n"
        printf "Got: \n"
        cat out.txt
        printf "\n====\n"
        failed_tests=$((failed_tests + 1))
        printf "<---------------------------------->\n"
    else
        passed_tests=$((passed_tests + 1))
    fi
    rm -rf ./.temp
    total_tests=$((total_tests + 1))
}

printf "Functional testing ...\n"

test_count=1
for file in test_pos/*
do
if [ $test_count == 1 ]
then
    file_argc=$file
fi

if [ $test_count == 2 ]
then 
    file_in=$file
fi

if [ $test_count == 3 ]
then
    file_out=$file
fi

test_count=$((test_count+1))

if [ $test_count == 4 ]
then
    test_count=1
    postest $file_argc $file_in $file_out
fi
done

test_count=1
for file in test_neg/*
do
if (( $test_count == 1 ))
then
    file_argc=$file
fi

if (( $test_count == 2 ))
then 
    file_in=$file
fi

if (( $test_count == 3 ))
then
    file_out=$file
fi

test_count=$((test_count+1))

if (( $test_count == 4 ))
then
    test_count=1
    negtest $file_argc $file_in $file_out
fi
done

# clean
rm out.txt $app_name
printf "Tests count: %d total, %d passed, %d failed.\n" $total_tests $passed_tests $failed_tests