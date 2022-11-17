#ifndef ExecuteMotion_H_INCLUDED_
#define ExecuteMotion_H_INCLUDED_

// GENERAL INCLUDES
#include <iostream>
#include <math.h>

// ROS INCLUDES
#include <ros/ros.h>

// BT includes.
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

// RECYCLA INCLUDES
#include <plan_motion.h>

/*This BT action node encapsulates the PAL ROBOTICS ARI "play_motion" functionality.*/

class ExecuteMotion : public BT::CoroActionNode
{
  public:  

    // Shared program resources.
    plan_motion* motion_;

    // ROS stuff.
    ros::NodeHandle             nh_;    
        
    // MoveIt! stuff.
    bool success_                 = false;

    // =================================================== 

    ExecuteMotion(const std::string& name, const BT::NodeConfiguration& config) : 
    BT::CoroActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        // This action has a single input port called "message"
        // Any port must have a name. The type is optional.
        return { BT::InputPort<std::string>("motion_name") };
                //  {BT::OutputPort<std::string>("move_group_")}};
    }

    void init(ros::NodeHandle*  input_nh,
              plan_motion*      input_motion)
    {
      std::cout << "### Initializing ExecuteMotion! ###" << std::endl;

      nh_       = *input_nh;
      motion_   = input_motion;
    }

    // You must override the virtual function tick()
    BT::NodeStatus      tick() override;
    void                cleanup(bool halted);
    void                halt() override;

    // =================================================== 

    // Execute Cartersian Path by specifying a list of waypoints to the EE
    // to go through.
    bool executeMotion(const std::string  &motion_name);
};
#endif // ExecuteMotion_H_INCLUDED_