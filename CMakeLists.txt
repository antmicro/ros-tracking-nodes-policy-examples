cmake_minimum_required(VERSION 2.8.3)
project(example_tracker)

add_definitions(-Wall -Wextra -Wmaybe-uninitialized -Wno-unused-result -Wno-unused-parameter -Werror)
set(CMAKE_CXX_STANDARD 17)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    cv_bridge
    sensor_msgs
    std_msgs
    message_generation
	genmsg
	tracking_tester
)

execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DCOMMIT="${GIT_COMMIT_HASH}")

catkin_package(
    INCLUDE_DIRS src
    LIBRARIES ${PROJECT_NAME}_lib
	CATKIN_DEPENDS tracking_tester
)

include_directories(
    src
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_LIBS}
)

add_executable(example_tracker
    src/main.cpp
)

add_dependencies(example_tracker ${catkin_EXPORTED_TARGETS})
add_dependencies(example_tracker ${PROJECT_NAME}_gencpp tracking_tester_generate_messages_cpp)


target_link_libraries(example_tracker
    ${catkin_LIBRARIES}
)
