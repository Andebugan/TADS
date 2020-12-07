#/bin/bash

posFname=pos.txt
negFname=neg.txt

test_dir="func_tests"

function create_tests() {
    filename=$1
    tType=$2
    files_dir="$test_dir/test_$tType"
    rm -rf $files_dir
    mkdir $files_dir
    test_count=1
    test_num=1
    while read line; do
        if [ $test_num -lt 10 ]
        then
            if [ $test_count == 1 ]
            then
                touch "${tType}_0${test_num}_args.txt"
                echo $line > "${tType}_0${test_num}_args.txt"
                mv "${tType}_0${test_num}_args.txt" $files_dir
            fi

            if [ $test_count == 2 ]
            then 
                touch "${tType}_0${test_num}_in.txt"
                echo $line > "${tType}_0${test_num}_in.txt"
                sed -i -e 's/~\+/\n/g' "${tType}_0${test_num}_in.txt"
                mv "${tType}_0${test_num}_in.txt" $files_dir
            fi

            if [ $test_count == 3 ]
            then
                test_count=0
                touch "${tType}_0${test_num}_out.txt"
                echo $line > "${tType}_0${test_num}_out.txt"
                sed -i -e 's/~\+/\n/g' "${tType}_0${test_num}_out.txt"
                mv "${tType}_0${test_num}_out.txt" $files_dir
                test_num=$((test_num+1))
            fi
            test_count=$((test_count+1))
        else
            if [ $test_count == 1 ]
            then
                touch "${tType}_${test_num}_args.txt"
                echo $line > "${tType}_${test_num}_args.txt"
                mv "${tType}_${test_num}_args.txt" $files_dir
            fi

            if [ $test_count == 2 ]
            then 
                touch "${tType}_${test_num}_in.txt"
                echo $line > "${tType}_${test_num}_in.txt"
                sed -i -e 's/~\+/\n/g' "${tType}_${test_num}_in.txt"
                mv "${tType}_${test_num}_in.txt" $files_dir
            fi

            if [ $test_count == 3 ]
            then
                test_count=0
                touch "${tType}_${test_num}_out.txt"
                echo $line > "${tType}_${test_num}_out.txt"
                sed -i -e 's/~\+/\n/g' "${tType}_${test_num}_out.txt"
                mv "${tType}_${test_num}_out.txt" $files_dir
                test_num=$((test_num+1))
            fi
            test_count=$((test_count+1))
        fi
    done < $filename
    printf "$tType: $((test_num-1))\n"
}

pos="pos"
neg="neg"

create_tests $posFname $pos
create_tests $negFname $neg