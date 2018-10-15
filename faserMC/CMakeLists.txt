option(DO_FULL_BUILD "Build full, Geant4-based code" $ENV{FASERMC_DO_FULL_BUILD})

#----------------------------------------------------------------------------
# Copy/pasted from G4 Example B1, with superficial change to project/exe name
# and some changes to installation
#
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(FASER)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
if(DO_FULL_BUILD)
  option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
  if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
  else()
    find_package(Geant4 REQUIRED)
  endif()
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
if(DO_FULL_BUILD)
  include(${Geant4_USE_FILE})
endif()

#---------------------------------------------------------------------------
# Find ROOT
#
find_package(ROOT REQUIRED)
# ROOT version 6 
if(ROOT_FOUND)
  STRING(REGEX MATCH "6.*" VERSION6MATCH ${ROOT_VERSION})
  if(NOT VERSION6MATCH)
    message(FATAL_ERROR "ROOT 6 is required")
  endif()
endif()

#----------------------------------------------------------------------------
# Setup ROOT include directories and compile definitions
#
include(${ROOT_USE_FILE})


#---------------------------------------------------------------------------
# Shared libraries are required (why?)
#
if(DO_FULL_BUILD)
  if(NOT Geant4_shared_FOUND)
    message(FATAL_ERROR "Geant must be built with shared libraries")
  endif()
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
if(DO_FULL_BUILD)
  include_directories(${PROJECT_SOURCE_DIR}/include
                      ${Geant4_INCLUDE_DIR}
                      ${ROOT_INCLUDE_DIR})
  file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
  file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
else()
  include_directories(${PROJECT_SOURCE_DIR}/include
                      ${ROOT_INCLUDE_DIR})
  file(GLOB sources ${PROJECT_SOURCE_DIR}/src/FaserTracker*.cc)
  file(GLOB headers ${PROJECT_SOURCE_DIR}/include/FaserTracker*.hh)
endif()

#---------------------------------------------------------------------------
# Generate dictionaries, add ROOT incantations
#
if(DO_FULL_BUILD)
  REFLEX_GENERATE_DICTIONARY(FaserRootClasses include/FaserRootClasses.hh SELECTION xml/selection.xml)
  add_library(FaserRootClassesDict SHARED ${sources} FaserRootClasses.cxx)
  set(libsuffix .so)
  set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
  set_target_properties(FaserRootClassesDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
  target_link_libraries(FaserRootClassesDict ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
endif()

REFLEX_GENERATE_DICTIONARY(FaserTrackerClasses include/FaserTrackerClasses.hh SELECTION xml/tracker_selection.xml)
add_library(FaserTrackerClassesDict SHARED ${sources} FaserTrackerClasses.cxx)
set(libsuffix .so)
set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
set_target_properties(FaserTrackerClassesDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
target_link_libraries(FaserTrackerClassesDict ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
if(DO_FULL_BUILD)
  add_executable(faser faser.cc ${sources} ${headers})
  target_link_libraries(faser FaserRootClassesDict ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
  target_link_libraries(faser FaserTrackerClassesDict ${ROOT_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build faser. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
if(DO_FULL_BUILD)
  set(FASER_SCRIPTS
    init_vis.mac
    init_novis.mac
    init_minion.mac
    vis.mac
    faserGeo.mac
    faserPhys.mac
    faserPhysMinIon.mac
    faserDigi.mac
    faserPhysicsList.mac
    )
  
  foreach(_script ${FASER_SCRIPTS})
    configure_file(
      ${PROJECT_SOURCE_DIR}/${_script}
      ${PROJECT_BINARY_DIR}/${_script}
      COPYONLY
      )
  endforeach()
endif()

#----------------------------------------
# Install root macros into a root subdirectory
#
if(DO_FULL_BUILD)
  set(FASER_ROOT_MACROS
      root/initFaser.C
      root/readTree.C
      root/FaserCluster.C
     )
  
  foreach(_macro ${FASER_ROOT_MACROS})
    configure_file(
      ${PROJECT_SOURCE_DIR}/${_macro}
      ${PROJECT_BINARY_DIR}/${_macro}
      COPYONLY
      )
  endforeach()
endif()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
if(DO_FULL_BUILD)
  add_custom_target(FASER DEPENDS faser)
endif()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
if(DO_FULL_BUILD)
  install(TARGETS faser DESTINATION bin)
  install(TARGETS FaserRootClassesDict DESTINATION lib64)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FaserRootClasses_rdict.pcm DESTINATION lib64)
  install(FILES ${FASER_SCRIPTS} DESTINATION .)
  install(FILES ${FASER_ROOT_MACROS} DESTINATION root)
endif()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FaserTrackerClasses_rdict.pcm DESTINATION lib64)
install(TARGETS FaserTrackerClassesDict DESTINATION lib64)

