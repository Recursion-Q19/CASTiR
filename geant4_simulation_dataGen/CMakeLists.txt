#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(phytoPET_sim) #takes the name of the folder and has to be different from the main executable

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14 -m64 -I/opt/ROOT/root_v6.12.06-MT-install/include")
#GEANT4 Version: 10.7
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

##### added by Aram to find ROOT ###
###===================================###
## This allows CMake to find your module
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

### Find ROOT
find_package(ROOT)

### Add ROOT header paths
include_directories(${ROOT_INCLUDE_DIR})

### Setup generation of dictionaries
set(MCEvent_SRCS ${PROJECT_SOURCE_DIR}/src/MCEvent.cc)

set(MCEvent_HEADERS ${PROJECT_SOURCE_DIR}/include/MCEvent.hh)

set(MCEvent_LINKDEF ${PROJECT_SOURCE_DIR}/include/MCEventLinkDef.h )
set(MCEvent_DICTIONARY ${PROJECT_SOURCE_DIR}/src/MCEventDict.cc) 

execute_process(COMMAND rootcint -f ${MCEvent_DICTIONARY} -c ${MCEvent_HEADERS} ${MCEvent_LINKDEF})

set(MCEvent_SRCS ${MCEvent_SRCS} ${MCEvent_DICTIONARY}) 
##set(sources ${souces} ${MCEvent_SRCS})   ###sometimes need to coment this out
set(headers ${headers} ${MCEvent_HEADERS})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(phytoPETsim phytoPETsim.cc ${sources} ${headers})
###target_link_libraries(phytoPETsim ${Geant4_LIBRARIES} )
target_link_libraries(phytoPETsim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build EXAMPLE. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(MY_SCRIPTS
    run.mac
    test.cdh
    macro/vis.mac 
    energy_spectra/pinacle_6MV.txt
    energy_spectra/pinacle_10MV.txt
    energy_spectra/pinacle_18MV.txt
    energy_spectra/O15-Blanco-Spectra.txt
    energy_spectra/F18-Blanco-Spectra.txt
    MCEventDict_rdict.pcm
  )

foreach(_script ${MY_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(phytoPET_sim DEPENDS phytoPETsim)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS phytoPETsim DESTINATION bin)












# add the dictionary to the list of source files

###SET(MCEvent_SRCS ${MCEvent_SRCS} ${MCEvent_DICTIONARY}) 

####add_library(MCEvent ${MCEvent_SRCS})
###target_link_libraries(MCEvent ${ROOT_LIBRARIES})

#add_custom_target(MCEvent DEPENDS ${MCEvent_SRCS})
#SET(sources ${souces} DEPENDS MCEvent)
