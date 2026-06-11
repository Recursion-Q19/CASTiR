#!/bin/bash

# echo Hello World! This is my script to automate the data preprocessing for STIR
# $ date 
# Wed 20 May 2026 07:53:58 PM CST

echo Please enter the path to the directory where the rotator executable resides \( absolute path \):
read ROT_EXE_DIR

echo Please enter the path to the directory where the appender executable resides \( absolute path \):
read COMBINER_EXE_DIR

echo Please enter the source files directory path \(absolute path\):
read SOURCE_DIR_INIT

echo Please enter the destination directory path \(where to copy\):
read DEST_DIR

echo Please enter the number of directories containing target files:
read cntr

echo Please enter which director number to start coping files:
read strt_dir

echo Please enter the path of the directory containing STIR files:
read PROJ_DATA_FILES


#This function copies each raw list-mode file (the direct output of simulation - simulation folder) to a new directory that has the same name as the simulation folder that contains this raw file.

CopyFunc(){
#for item in {1..n};if n is a variable we have to use the following syntax 
for index in $(seq $cntr)
do
	local iterator=$((index + strt_dir - 1))
	
	local ADDED_PATH=/build${iterator}/test.clm.safir
	local SOURCE_FILE_DIR=${SOURCE_DIR_INIT}${ADDED_PATH}

	cat << EOF
$SOURCE_FILE_DIR
EOF
	mkdir -p ${DEST_DIR}/build${iterator}/
	cp -iv  $SOURCE_FILE_DIR ${DEST_DIR}/build${iterator}/
done
}


RotationFunc(){

ROT_EXEC_PATH=${ROT_EXE_DIR}/rotLM-STIR

chmod u+x "$ROT_EXEC_PATH"

echo Please enter the number of rotations:
read num_rots

for index in $(seq $cntr)
do
	local iterator=$((index + strt_dir - 1))
	
	echo Now rotating build${iterator}
	
	for item in $(seq $num_rots)
	do
		$ROT_EXEC_PATH -rot ${item} -inF ${DEST_DIR}/build${iterator}/test.clm.safir -outF ${DEST_DIR}/build${iterator}/rotated${item}.clm.safir 	
	done
done
}


CombinerFunc(){

COMB_EXEC_PATH=${COMBINER_EXE_DIR}/AppenderLM-STIR

chmod u+x "$COMB_EXEC_PATH"

for index in $(seq $cntr)
do	
	local iterator=$((index + strt_dir - 1))
	
	for item in $(seq $num_rots)
	do
		$COMB_EXEC_PATH -inF1 ${DEST_DIR}/build${iterator}/test.clm.safir -inF2 ${DEST_DIR}/build${iterator}/rotated${item}.clm.safir	
	done
done
}

ProjDataTransformerFunc(){


for index in $(seq $cntr)
do	
	local iterator=$((index + strt_dir - 1))
	
	cp -iv  $PROJ_DATA_FILES/listmode_input_SAFIR.par ${DEST_DIR}/build${iterator}/
	cp -iv  $PROJ_DATA_FILES/MySTIR_Simulation_crystal_map.txt ${DEST_DIR}/build${iterator}/
	cp -iv  $PROJ_DATA_FILES/MySTIR_Simulation.hs ${DEST_DIR}/build${iterator}/
	
	touch ${DEST_DIR}/build${iterator}/MySTIR_Simulation.s
	
	
	if true; then
		cat >> ${DEST_DIR}/build${iterator}/lm_to_projdata.par << EOF
lm_to_projdata Parameters:=

  input file := listmode_input_SAFIR.par
  output filename prefix := ${iterator}-MySTIR_Simulation

  ; parameters that determine the sizes etc of the output

	template_projdata := MySTIR_Simulation.hs
	; the next can be used to use a smaller number of segments than given
	; in the template
	maximum absolute segment number to process :=

  ; parameters for saying which events will be stored

	; time frames (see TimeFrameDefinitions doc for format)
	; frame_definition file := frames.fdef
	; or a total number of events (if  larger than 0, frame definitions will be ignored)
	; note that this normally counts the total of prompts-delayeds (see below)
	num_events_to_store := 10000000000

  ; parameters relating to prompts and delayeds

	; with the default values, prompts will be added and delayed subtracted
	; to give the usual estimate of the trues.

	; store the prompts (value should be 1 or 0)
	store prompts := 1  ;default
	; what to do if it's a delayed event
	store delayeds := 0  ;default

  ; miscellaneous parameters

	; list each event on stdout and do not store any files (use only for testing!)
	; has to be 0 or 1
	List event coordinates := 0 ;set to 1 if you want to see stored records one by one in the terminal

	; if you're short of RAM (i.e. a single projdata does not fit into memory),
	; you can use this to process the list mode data in multiple passes.
	num_segments_in_memory := -1

End :=
EOF
	fi
	
	cd ${DEST_DIR}/build${iterator}/
	
	lm_to_projdata lm_to_projdata.par
done
}

echo Copying Starts ... 
CopyFunc

echo Rotating Starts ...
RotationFunc

echo Combining Starts ...
CombinerFunc

echo lm_to_projdata Runs
ProjDataTransformerFunc

echo The End
