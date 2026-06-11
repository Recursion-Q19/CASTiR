#!/bin/bash

#echo Hello World! This is my First Script which is used for CASToR normalization file movement - Aug 19, 2025 at 9:00

CopyFunc(){

echo Please Enter the home directory path:
read HOME_DIR_INIT

echo Please Enter the destination directory path:
read DEST_DIR

echo Please Enter the number of directories containing target files:
read cntr

#for item in {1..n};if n is a variable we have to use the following syntax 
for item in $(seq $cntr)
do
	ADDED_PATH=/build_trick${item}/log.txt
	HOME_DIR=${HOME_DIR_INIT}${ADDED_PATH}

	cat << EOF
$HOME_DIR
EOF

	cp -iv  $HOME_DIR ${DEST_DIR}/log${item}.txt
done

}


FileExecFunc(){

EXEC_PATH=${DEST_DIR}/textCombiner

	chmod u+x $EXEC_PATH

for item in {1..4}
do

	local index1=$((item + 1))
	local index2=$((item + cntr))
	local index3=$((item + cntr + 1))
	$EXEC_PATH ${DEST_DIR}/log${index1}.txt ${DEST_DIR}/log${index2}.txt ${DEST_DIR}/log${index3}.txt	
done

}

CopyFunc

mv -iv  ${DEST_DIR}/log1.txt ${DEST_DIR}/log6.txt

FileExecFunc
