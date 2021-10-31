cmake_minimum_required(VERSION 3.20)
project(IRC)

set(CMAKE_CXX_STANDARD 98)
set(CC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CC_COVERAGE_COMPILE_FLAGS}" )
add_executable(IRC  main.cpp channel.cpp	./server/Server.cpp	user.cpp utils.cpp split.cpp
./command/pingPong.cpp ./command/quit.cpp ./command/userCmd.cpp ./command/part.cpp
./command/privmsg.cpp ./command/nick.cpp ./command/join.cpp ./command/mode.cpp
./command/oper.cpp ./command/kick.cpp ./command/notice.cpp ./command/list.cpp ./command/invite.cpp ./command/topic.cpp)
