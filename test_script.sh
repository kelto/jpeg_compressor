#!/bin/bash

ko='\e[00;31m';
wipe='\e[00m';
ok='\e[01;32m';

BASE=.
TEST=./test_files
RET=

function test_dct {
    if [ -x $BASE/compressor ]
    then
	rm -rf $TEST/test_dct;
	mkdir -p $TEST/test_dct;

	cat $TEST/file_list.txt | while read i
	do
	    $BASE/compressor 2 $TEST/$i $TEST/test_dct/$i
	done;
	
	for i in $(cat $TEST/file_list.txt)
	do
	    diff $TEST/ref_test_dct/$i $TEST/test_dct/$i >/dev/null
	    RET=$?
            if [ $RET -ne 0 ]
	    then
		break
	    fi
	done
    else
	echo "command not found;"
	RET=2
    fi
}


function test_quantify {
    if [ -x $BASE/compressor ]
    then
	rm -rf $TEST/test_quantify;
	mkdir -p $TEST/test_quantify;

	cat $TEST/file_list.txt | while read i
	do
	    $BASE/compressor 3  $TEST/$i $TEST/test_quantify/$i
	done;
	
	for i in $(cat $TEST/file_list.txt)
	do
	    diff $TEST/ref_test_quantify/$i $TEST/test_quantify/$i >/dev/null
	    RET=$?
            if [ $RET -ne 0 ]
	    then
		break
	    fi
	done
    else
	echo "command not found;"
	RET=2
    fi
}


function test_vectorize {
if [ -x $BASE/compressor ]
then
rm -rf $TEST/test_vectorize;
mkdir -p $TEST/test_vectorize;

cat $TEST/file_list.txt | while read i
do
$BASE/compressor 4  $TEST/$i $TEST/test_vectorize/$i
done;

for i in $(cat $TEST/file_list.txt)
do
diff $TEST/ref_test_vectorize/$i $TEST/test_vectorize/$i >/dev/null
RET=$?
if [ $RET -ne 0 ]
then
break
fi
done
else
echo "command not found;"
RET=2
fi
}


function test_compression {
    if [ -x $BASE/compressor ]
    then
	rm -rf $TEST/test_compression;
	mkdir -p $TEST/test_compression;

	cat $TEST/file_list.txt | while read i
	do
	    $BASE/compressor 1  $TEST/$i $TEST/test_compression/$i.xxx
	done;
	
	for i in $(cat $TEST/file_list.txt)
	do
	    diff $TEST/ref_test_compression/${i%.pgm}.xxx $TEST/test_compression/${i%.pmg}.xxx >/dev/null
	    RET=$?
            if [ $RET -ne 0 ]
	    then
		break
	    fi
	done
    else
	echo "command not found;"
	RET=2
    fi
}

function test_decompression {
    if [ -x $BASE/compressor ]
    then
	rm -rf $TEST/test_decompression;
	mkdir -p $TEST/test_decompression;

	cat $TEST/file_list.txt | while read i
	do
        echo "file : "$i
	    $BASE/compressor 1  $TEST/$i $TEST/test_decompression/$i.xxx
	    $BASE/compressor 0  $TEST/test_decompression/$i.xxx $TEST/test_decompression/$i
	done;
	
	for i in $(cat $TEST/file_list.txt)
	do
	    diff $TEST/ref_test_decompression/$i $TEST/test_decompression/$i >/dev/null
	    RET=$?
            if [ $RET -ne 0 ]
	    then
		break
	    fi
	done
    else
	echo "command not found;"
	RET=2
    fi
}

function test_diff {
    if [ -x $BASE/compressor ]
    then
	rm -rf $TEST/test_quantify;
	mkdir -p $TEST/test_quantify;

	cat $TEST/file_list.txt | while read i
	do
	    $BASE/compressor 5  $TEST/$i dummy.xxx
	done;
	else
	echo "command not found;"
	RET=2
    fi

	}

function test {
	test_$1
	[ $RET -eq 0 ] && printf "%-12s [${ok}OK${wipe}]\n" "$1" 
	[ $RET -ne 0 ] && printf "%-12s [${ko}KO${wipe}]\n" "$1" 
}

test dct;
test quantify;
test vectorize;
test compression;
test decompression;
test diff;
exit 0
