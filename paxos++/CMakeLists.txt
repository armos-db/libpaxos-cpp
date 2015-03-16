CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

PROJECT(libpaxos)

SET(CMAKE_USE_RELATIVE_PATHS ON)

SET(libpaxos_src
	client.cpp
	server.cpp)
	
ADD_LIBRARY(libpaxos SHARED ${libpaxos_src})
