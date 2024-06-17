FILE(GLOB HEADERS *.h)

set(OPT_CXX_FLAGS "/openmp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_CXX_FLAGS}")

add_executable(melody2d main.cpp)
SET_PROPERTY(TARGET melody2d PROPERTY PUBLIC_HEADER ${HEADERS})
add_executable(batchchains BATCHCHAINS.cpp)
add_executable(batchcontours BATCHCONTOURS.cpp)
add_executable(batchextract BATCHEXTRACT.cpp)
add_executable(batchfields BATCHFIELDS.cpp)
add_executable(batchprint BATCHPRINT.cpp)

INSTALL(TARGETS melody2d batchchains batchcontours batchextract batchfields batchprint
RUNTIME DESTINATION bin 
ARCHIVE DESTINATION lib
PUBLIC_HEADER DESTINATION include)