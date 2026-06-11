#!/bin/bash

echo =======================================================================================
echo *****Note that the crystal map should be in the same directory as this executable *****
#or we need to update the .hs file in a way that contains the path to where the crystal map resides.
echo =======================================================================================

echo enter source directory:
read SOURCE_DIR

echo enter destination directory:
read DEST_DIR

declare -a chunks

for item in {1..30}
do
	chunks[${item}]="${SOURCE_DIR}/build${item}/${item}-MySTIR_Simulation_f1g1d0b0.hs"
	echo $item -- ${chunks[${item}]}
done

#how to add the following into a list that can be passed to the following command without the need to type each individual separately.

stir_math -s ${DEST_DIR}/normalizationData.hs ${chunks[1]} ${chunks[2]} ${chunks[3]} ${chunks[4]} ${chunks[5]} ${chunks[6]} ${chunks[7]} ${chunks[8]} ${chunks[9]} ${chunks[10]} ${chunks[11]} ${chunks[12]} ${chunks[13]} ${chunks[14]} ${chunks[15]} ${chunks[16]} ${chunks[17]} ${chunks[18]} ${chunks[19]} ${chunks[20]} ${chunks[21]} ${chunks[22]} ${chunks[23]} ${chunks[24]} ${chunks[25]} ${chunks[26]} ${chunks[27]} ${chunks[28]} ${chunks[29]} ${chunks[30]}
