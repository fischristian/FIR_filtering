###################################
###### (R) Christian Fischer, 2015
###### fis.christian@web.de
###################################

1. CONTENT
2. REQUIREMENTS
3. HOW TO INSTALL

1. CONTENT
###################################
- bin
  // output directory
- doc:
  // documentation folder
    - 00_Requirements.txt
	  // brief summary of requirements
	- 01_Evaluation.txt
	  // brief summary of test results
	- 02_Outlook.txt
	  // brief summary of further improvements
	
- filter_dll:
  // implementation of filter functionality
    - include
	    - FilterAPI.h
		  // filter header file
		- FilterStatistics.h
		  // filter statistics header file
	- source
	    - FilterAPI.cpp
		  // filter source file
		- FilterStatistics.cpp
		  // filter statistics source file
	- CMakeLists.txt
	  // filter library CMake file
	
- test_app:
  // implementation of main routine and api and performance tests
    - include
	    - Application.h
		  // application header file
		- Testcases.h
		  // testcases header file
	- source
	    - Application.cpp
		  // application source file
		- Testcases.cpp
		  // testcases source file
		- main.cpp
		  // main routine source file
	- CMakeLists.txt
	  // application CMake file
	
- CMakeLists.txt
  // main CMake file

- README.txt
  // THIS file


2. REQUIREMENTS
###################################
- CMake Version 2.8.9
- c++11 compatible compiler
  // testet with VS2013, gcc 4.8.2
- Images must be a binary file with 8 Bit grey-value each pixel

3. HOW TO INSTALL
###################################
- Start CMake-Gui
- Select as Source dir ("Where is the source code"): /FIR_filtering
- Select as Output dir ("Where to build the binaries"): /FIR_filtering/bin
- Click "Configure": Select you compiler / environment
- Click "Generate"
- Open folder /FIR_filtering/bin
- Run make or open and compile VS solution

