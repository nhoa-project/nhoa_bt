#ifndef __PLANNING_NAVIGATION_H_INCLUDED__
#define __PLANNING_NAVIGATION_H_INCLUDED__

// ROS headers
#include <ros/ros.h>
#include "tf/transform_listener.h"
#include "actionlib/client/simple_action_client.h"
#include "move_base_msgs/MoveBaseAction.h"

// =====

class plan_navigation
{
private:
    //vars
    // ROS stuff
    ros::NodeHandle     nh_;

    // Actionlib stuff.
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> client_;

    // Move base stuff.
    move_base_msgs::MoveBaseGoal goal_;
    bool                         action_status_;

    // =======
    //functions
    // Initializing.
    void init();

    // Cook navigation.
    void cook_navigation(const std::vector<double>    &navigation_goal);

public:
    //vars

    // =======
    //functions
    plan_navigation(ros::NodeHandle    *nodehandle);

    bool set_navigation_goal(const std::vector<double>    &navigation_goal);
};
#endif // __PLANNING_NAVIGATION_H_INCLUDED__