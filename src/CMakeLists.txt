#this is just a basic CMakeLists.txt, for more information see the cmake manpage

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2 -ggdb)

#build a shared library
ADD_LIBRARY(nightget SHARED NightGetQueues.cpp NightGetNodes.cpp NightGetDownload.cpp NightGetController.cpp NightGetUtilities.cpp)

#for testing the shared library you probably need some test app too
ADD_EXECUTABLE(nightgettest nightgettest.cpp)

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES(nightgettest nightget mysqlclient_r curl pthread)

#add an install target here
#INSTALL_FILES(...)
#INSTALL_PROGRAMS(...)
#INSTALL_TARGET(...)

