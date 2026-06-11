#!/bin/bash

#echo Hello World! This is my First Script which is used for CASToR normalization file movement - Aug 19, 2025 at 9:00

CopyFunc(){

echo Please Enter the executable directory path:
read EXEC_DIR

echo Please Enter the source files directory path:
read SOURCE_DIR

echo Please Enter the destination directory path:
read DEST_DIR

echo Please Enter the number of directories containing target files:
read cntr

echo Please enter the number of rotations:
read num_rots

#for item in {1..n};if n is a variable we have to use the following syntax 
for item in $(seq $cntr)
do
	ADDED_PATH=/build${item}/test.cdf
	SOURCE_FILES_DIR=${SOURCE_DIR}${ADDED_PATH}

	cat << EOF
$SOURCE_FILES_DIR
EOF

	cp -iv  $SOURCE_FILES_DIR ${DEST_DIR}/test${item}.cdf
done

}


CombineFunc1(){

COMB_EXEC_PATH=${EXEC_DIR}/castorHistDataCombiner

	chmod u+x $COMB_EXEC_PATH

	echo "$COMB_EXEC_PATH -outF ${DEST_DIR}/test.cdf -inF ${DEST_DIR}/test1.cdf ${DEST_DIR}/test2.cdf ${DEST_DIR}/test3.cdf ${DEST_DIR}/test4.cdf ${DEST_DIR}/test5.cdf ${DEST_DIR}/test6.cdf ${DEST_DIR}/test7.cdf ${DEST_DIR}/test8.cdf ${DEST_DIR}/test9.cdf ${DEST_DIR}/test10.cdf ${DEST_DIR}/test11.cdf ${DEST_DIR}/test12.cdf ${DEST_DIR}/test13.cdf ${DEST_DIR}/test14.cdf ${DEST_DIR}/test15.cdf ${DEST_DIR}/test16.cdf ${DEST_DIR}/test17.cdf ${DEST_DIR}/test18.cdf ${DEST_DIR}/test19.cdf ${DEST_DIR}/test20.cdf ${DEST_DIR}/test21.cdf ${DEST_DIR}/test22.cdf ${DEST_DIR}/test23.cdf ${DEST_DIR}/test24.cdf ${DEST_DIR}/test25.cdf ${DEST_DIR}/test26.cdf ${DEST_DIR}/test27.cdf ${DEST_DIR}/test28.cdf ${DEST_DIR}/test29.cdf ${DEST_DIR}/test30.cdf"
	
	echo Started
	
	$COMB_EXEC_PATH -outF ${DEST_DIR}/test.cdf -inF ${DEST_DIR}/test1.cdf ${DEST_DIR}/test2.cdf ${DEST_DIR}/test3.cdf ${DEST_DIR}/test4.cdf ${DEST_DIR}/test5.cdf ${DEST_DIR}/test6.cdf ${DEST_DIR}/test7.cdf ${DEST_DIR}/test8.cdf ${DEST_DIR}/test9.cdf ${DEST_DIR}/test10.cdf ${DEST_DIR}/test11.cdf ${DEST_DIR}/test12.cdf ${DEST_DIR}/test13.cdf ${DEST_DIR}/test14.cdf ${DEST_DIR}/test15.cdf ${DEST_DIR}/test16.cdf ${DEST_DIR}/test17.cdf ${DEST_DIR}/test18.cdf ${DEST_DIR}/test19.cdf ${DEST_DIR}/test20.cdf ${DEST_DIR}/test21.cdf ${DEST_DIR}/test22.cdf ${DEST_DIR}/test23.cdf ${DEST_DIR}/test24.cdf ${DEST_DIR}/test25.cdf ${DEST_DIR}/test26.cdf ${DEST_DIR}/test27.cdf ${DEST_DIR}/test28.cdf ${DEST_DIR}/test29.cdf ${DEST_DIR}/test30.cdf
}

RotateFunc(){

ROT_EXEC_PATH=${EXEC_DIR}/coincidenceTransfermer

	chmod u+x $ROT_EXEC_PATH
	
	echo Now rotating build${iterator}
	
	for item in $(seq $num_rots)
	do
		local index=$((item - 1))
		
		echo "$ROT_EXEC_PATH -inF ${DEST_DIR}/test${index}.cdf -outF ${DEST_DIR}/rotated${item}.cdf"
		
		echo "Started ${index}"
		
		$ROT_EXEC_PATH -inF ${DEST_DIR}/rotated${index}.cdf -outF ${DEST_DIR}/rotated${item}.cdf
		
	done

}

CombineFunc2(){
	echo "$COMB_EXEC_PATH -outF ${DEST_DIR}/inData_CASToR.cdf -inF ${DEST_DIR}/rotated0.cdf ${DEST_DIR}/rotated1.cdf ${DEST_DIR}/rotated2.cdf ${DEST_DIR}/rotated3.cdf ${DEST_DIR}/rotated4.cdf ${DEST_DIR}/rotated5.cdf ${DEST_DIR}/rotated6.cdf ${DEST_DIR}/rotated7.cdf"

	echo Started

	$COMB_EXEC_PATH -outF ${DEST_DIR}/inData_CASToR.cdf -inF ${DEST_DIR}/rotated0.cdf ${DEST_DIR}/rotated1.cdf ${DEST_DIR}/rotated2.cdf ${DEST_DIR}/rotated3.cdf ${DEST_DIR}/rotated4.cdf ${DEST_DIR}/rotated5.cdf ${DEST_DIR}/rotated6.cdf ${DEST_DIR}/rotated7.cdf
}

CopyFunc

CombineFunc1

cp -iv  ${DEST_DIR}/test.cdf ${DEST_DIR}/rotated0.cdf

RotateFunc

CombineFunc2
