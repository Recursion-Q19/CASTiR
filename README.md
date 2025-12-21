This project provides open-source tools and establishes benchmark for comparing the two well-known image reconstruction software; i.e, [_**STIR**_](https://stir.sourceforge.net/) and [__*CASToR*__](https://castor-project.org/) in some specific setups.  However, a general comparison is hard to do in one work due to the different purposes, nature, and needs. This project provides a complete pipeline from data generation using a single [___GEANT4___](https://geant4.web.cern.ch/) simulation to tools and documentation for image reconstruction using _STIR_ and *CASToR* as well as image analysis tools. The tools in this repository might be updated in the future. 

For this study, we have used the following versions of the reconstruction software.

* _STIR_ version 3.2
* _CASToR_ version 6.2

The project contains the following content.

---

#### 1. Simulation: [geant4\_simulation\_dataGen](https://github.com/Recursion-Q19/CASTiR/tree/main/geant4_simulation_dataGen)

Our _GEANT4_ Simulation outputs two binary files with the same identical data in list-mode format, each of which is compatible for one of the software. For this project, the use of _GEANT4_ is to model the positron range as well as the energy spectra and scattering to obtain a more realistic simulation. Although we have simulated the scanner, phantom and record the hits, these things can be done through any other techniques. But *GEANT4* has given us a unified environment to do all together and help us to build on top for future projects, research and developments. You can learn more about it in the [README.md file](https://github.com/Recursion-Q19/CASTiR/blob/main/geant4_simulation_dataGen/README.md) for that directory. 

- In this project we have used _GEANT4_ version 10.4.7

---

#### 2. Tools for input files: [input\_file\_tools](https://github.com/Recursion-Q19/CASTiR/tree/main/input_file_tools) 

This directory contains some tools for parsing the binary input files for *STIR* and *CASToR*. These include (C++) codes for parsing the binary input files for each software, for combining two input files for each software, and instructions on how to prepare the header input file for each software. Some of these tools could have been implemented in _Geant4_ simulation directly. However, we decided to provide them separately that might be useful for any binary file. Note that some of these files and instructions are specific for this project. For other cases, it might need to be modified. We have tried to provided instructions for other cases in each file or _README.me_ if any.

---

#### 3. ImageJ Macros for image analysis: [ImageTools](https://github.com/Recursion-Q19/CASTiR/tree/main/ImageTools)

In this project to view and analyze images, we have used [***ImageJ***](https://imagej.net/). This directory contains a few ImageJ macros that are useful for analyzing volumetric images obtained through reconstruction. One macro is for calculating the [spatial resolution](https://github.com/Recursion-Q19/CASTiR/blob/main/ImageTools/NEMA-like_spatial_resolution.ijm), and the other one is for [image quality](https://github.com/Recursion-Q19/CASTiR/blob/main/ImageTools/ImageQuality.ijm) computation. These macros are provided rather as templates. You need to customize it for your specific needs. Almost everything is explained in each file as comments on how to change them for a specific need. 

---

#### 4. Reconstruction: [reconstruction](https://github.com/Recursion-Q19/CASTiR/blob/main/reconstruction)
This directory provides sample images reconstructed by each reconstruction software along with some parameter files as well as log files of the reconstruction processes output by each software. The instruction on how to run each reconstruction software is included in this directory.

---

#### 5. Report: [CASToRvsSTIR](https://github.com/Recursion-Q19/CASTiR/blob/main/CASToRvsSTIR.pdf)
This is an article reporting our results. This work have been accepted in the [Symposium on AI and Reconstruction for Biomedical Imaging (9 - 10 March, 2026)](https://www.ccpsynerbi.ac.uk/events/airbi/), London, UK. This file is the extended/full version of what submitted to the venue. 

---

#### Resources:
Some other repositories that are relevant and/or provide useful resources (this will be updated).

<ol style="list-style-type: none;">
	<li> STIR: <a href="https://github.com/UCL/STIR">https://github.com/UCL/STIR</a> </li>
	<li> CASToR: <a href="https://castor-project.org/">https://castor-project.org/</a> </li>
	<li> Pet Image Tools: <a href="https://github.com/Recursion-Q19/pet\_image\_tools/tree/main/ImageJ">https://github.com/Recursion-Q19/pet_image_tools/tree/main/ImageJ</a> </li>
	<li> Geant4: <a href="https://github.com/Geant4/geant4">https://github.com/Geant4/geant4</a> </li>
	<li> ImageJ: <a href="https://github.com/imagej">https://github.com/imagej</a>, <a href="https://imagej.net/">https://imagej.net/</a>, <a href="https://imagej.net/ij/">https://imagej.net/ij/</a> </li>
</ol>

#### Installation
To install each software, please refer to their websites and official documentation.
