cmake_minimum_required(VERSION 3.5)
project(nhoa_bt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
add_compile_options(-g)

## is used, also find other catkin packages
if(POLICY CMP0057)
    cmake_policy(SET CMP0057 NEW)
endif()

find_package(catkin REQUIRED COMPONENTS 
  actionlib 
  behaviortree_cpp_v3
  control_msgs 
  controller_manager_msgs
  diagnostic_msgs 
  diagnostic_updater 
  moveit_ros_planning_interface 
  move_base_msgs
  nav_msgs
  roscpp 
  sensor_msgs
  trajectory_msgs
  # PAL msgs
  hri_msgs
  pal_interaction_msgs
  pal_web_msgs
  play_motion_msgs 
  # CUSTOM msgs
  nhoa_msgs
  upo_sgg_msgs
  nhoa_approach_action
  nhoa_head_following_action
  )
  
find_package(Boost REQUIRED COMPONENTS thread)

catkin_package(INCLUDE_DIRS include
               LIBRARIES play_motion_helpers
               CATKIN_DEPENDS actionlib control_msgs controller_manager_msgs
               moveit_ros_planning_interface roscpp sensor_msgs 
               pal_interaction_msgs pal_web_msgs play_motion_msgs hri_msgs  # PAL msgs
               nhoa_msgs upo_sgg_msgs nhoa_approach_action nhoa_head_following_action) # CUSTOM msgs 


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include/nhoa_bt
  include/bt_action_nodes_h
  include/bt_condition_nodes_h)
include_directories(SYSTEM ${catkin_INCLUDE_DIRS})

link_directories(
  include/nhoa_bt
  include/bt_action_nodes_h
  include/bt_condition_nodes_h
)

# BT NODES
add_executable(${PROJECT_NAME}_demo src/${PROJECT_NAME}_demo.cpp 
               # BT ACTION NODES.
               src/bt_action_nodes/CancelNavigationGoal.cpp
               src/bt_action_nodes/CheckSmileScore.cpp
               src/bt_action_nodes/DrawUserAttention.cpp
               src/bt_action_nodes/ExecuteConversation.cpp
               src/bt_action_nodes/ExecuteHeadMotion.cpp
               src/bt_action_nodes/ExecuteGUI.cpp
               src/bt_action_nodes/ExecuteNavigation.cpp
               src/bt_action_nodes/ExecuteUperbodyMotion.cpp
               src/bt_action_nodes/ExecuteVoiceCmd.cpp
               src/bt_action_nodes/UploadUserInput.cpp
               # BT CONDITION NODES.
               src/bt_condition_nodes/IsApproximationReached.cpp
               src/bt_condition_nodes/IsBottleOnTable.cpp
               src/bt_condition_nodes/IsUserAnswered.cpp
               src/bt_condition_nodes/IsUserDetected.cpp
               src/bt_condition_nodes/IsUserEngaged.cpp
               src/bt_condition_nodes/IsUserEngaging.cpp
               src/bt_condition_nodes/IsUserSeated.cpp
               #############################
               src/handle_gui.cpp
               src/handle_hri.cpp
               src/handle_voice.cpp
               src/handle_scene.cpp
               src/plan_head_motion.cpp
               src/plan_motion.cpp
               src/plan_navigation.cpp
               )
add_dependencies(${PROJECT_NAME}_demo ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}_demo ${catkin_LIBRARIES})