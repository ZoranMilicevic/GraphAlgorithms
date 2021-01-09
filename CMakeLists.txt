cmake_minimum_required(VERSION 2.4)
set(CMAKE_CXX_STANDARD 17)

project(GraphAlgorithms)

set(HEADERS
	inc/BFS_MT_Command.h
	inc/BFS_ST_Command.h
	inc/DequeThreadsafe.h
	inc/DFS_MT_Command.h
	inc/DFS_ST_Command.h
	inc/Graph.h
	inc/MST_MT_Command.h
	inc/MST_ST_Command.h
	inc/MST_Tree.h
	inc/ResultReport.h
	inc/Semaphore.h
	inc/ServerCommand.h
	inc/ThreadPool.h
	inc/VisitedArray.h
	inc/VisitedArrayThreadsafe.h
)

set(SOURCES
	src/BFS_MT_Command.cpp
	src/BFS_ST_Command.cpp
	src/DFS_MT_Command.cpp
	src/DFS_ST_Command.cpp
	src/Graph.cpp
	src/MST_MT_Command.cpp
	src/MST_ST_Command.cpp
	src/MST_Tree.cpp
	src/ResultReport.cpp
	src/Server.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/rpclib/include
)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} 
		debug ${CMAKE_CURRENT_SOURCE_DIR}/libs/rpclib/Windows/x64/Debug/rpc.lib
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/libs/rpclib/Windows/x64/Release/rpc.lib
	)
endif (WIN32)

if (UNIX)
	target_link_libraries(${PROJECT_NAME} 
		debug ${CMAKE_CURRENT_SOURCE_DIR}/libs/rpclib/Linux/x64/Debug/rpc.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/libs/rpclib/Linux/x64/Release/rpc.a
	)
endif (UNIX)