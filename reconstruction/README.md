How to run:
- CAToR: 

	```bash
	$ castor-recon -df test.cdh -opti MLEM -it 6:4 -proj joseph -conv gaussian,0.5,0.5,3::psf -dim 241,241,160 -vox 0.25,0.25,0.65 -dout CASToR
	```
	
- STIR:

	```bash
	$ lm_to_projdata lm_to_projdata.par
	```
	
	Then
	
	```bash
	$ OSMAPOSL MySimulation_OSMAPOSL.par
	```
	
	* MySTIR_Simulation_f1g1d0b0.s actually was of size 384.2 MB (384,160,000 bytes). However, it is replaced in this project with an empty file because of size limit. This empty file is for demonstration. 
