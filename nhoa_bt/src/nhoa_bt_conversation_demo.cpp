// BT INCLUDES
#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_minitrace_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_file_logger.h>
#include <behaviortree_cpp_v3/bt_factory.h>

// ROS INCLUDES
#include <ros/ros.h>
#include <ros/package.h>

// STD INCLUDES
#include <vector>
#include <string>

// ZMQ INCLUDES
// #ifdef ZMQ_FOUND
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h"
// #endif

#include <fstream>

#define MANUAL_STATIC_LINKING

#ifdef MANUAL_STATIC_LINKING

// ACTION NODES
#include <ExecuteConversation.h>
#include <ExecuteVoiceCmd.h>
#include <UploadUserInput.h>

// CONDITION NODES
#include <IsQuestionnaireRequested.h>

#endif

using namespace BT;

int main(int argc, char **argv)
{
    // Initialize ROS
    ros::init(argc, argv, "nhoa_bt_conversation_demo");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(0);
    spinner.start();

    // Initialize resources [TODO: Add new instances here].

    // Load xml BT file
    std::string default_path = ros::package::getPath("nhoa_bt");
    std::string default_local_path = "/include/nhoa_bt/BT_XML/NHoAConversationDemoBT.xml";

    default_path += default_local_path;
    std::string path = nh.param("bt_xml_path", default_path); //If param ok, the path is completely defined (package+local path), if not default path is calculated
    ROS_INFO_STREAM("Path: " << path);
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        ROS_ERROR("File not open");
        return 0;
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    std::string xml_text = buffer.str();


    // We use the BehaviorTreeFactory to register our custom nodes
    BehaviorTreeFactory factory;
    
#ifdef MANUAL_STATIC_LINKING
    // Note: the name used to register should be the same used in the XML.
    // Note that the same operations could be done using DummyNodes::RegisterNodes(factory)

    // The recommended way to create a Node is through inheritance.
    // Even if it requires more boilerplate, it allows you to use more functionalities
    // like ports (we will discuss this in future tutorials).

    std::cout << "BT_CONTROL: Starting the node registration" << std::endl;

    // ACTION NODES (BT full implemented).
    factory.registerNodeType<ExecuteConversation>("ExecuteConversation");
    factory.registerNodeType<ExecuteVoiceCmd>("ExecuteVoiceCmd");
    factory.registerNodeType<UploadUserInput>("UploadUserInput");

    // ACTION NODES (Class + BT wrapped functionalities).    
    // factory.registerSimpleAction("ResetWrist3",
    //                              std::bind(&wrist_3_arm_movements::ResetWrist3, &wrist3));

    // CONDITION NODES (BT full implemented).
    // factory.registerNodeType<ConditionNodeFrame>("ConditionNodeFrame");
    factory.registerNodeType<IsQuestionnaireRequested>("IsQuestionnaireRequested");

    // CONDITION NODES (Class + BT wrapped functionalities).
    // factory.registerSimpleCondition("IsGrasping",
    //                                 std::bind(&gripper_status::IsGrasping, &gripper_status));

    std::cout << "BT_CONTROL: All nodes registered" << std::endl;

#else
    // Load dynamically a plugin and register the TreeNodes it contains
    // it automated the registering step.
    //factory.registerFromPlugin("./libdummy_nodes_dyn.so");
#endif

    // Trees are created at deployment-time (i.e. at run-time, but only once at the beginning).
    // The currently supported format is XML.
    // IMPORTANT: when the object "tree" goes out of scope, all the TreeNodes are destroyed
    auto tree = factory.createTreeFromText(xml_text);
    
    // This logger prints state changes on console
    StdCoutLogger logger_cout(tree);

    // This logger stores the execution time of each node
    MinitraceLogger logger_minitrace(tree, "nhoa_bt_conversation_demo_trace.json");

    // This logger saves state changes on file
    FileLogger logger_file(tree, "nhoa_bt_conversation_demo_bt_trace.fbl");

// #ifdef ZMQ_FOUND

    // This logger publish status changes using ZeroMQ. Used by Groot
    PublisherZMQ publisher_zmq(tree);
    std::cout << "BT COMMUNICATION: ZMQ found" << std::endl;
    
// #endif

    std::cout << "Print tree recursively!" << std::endl;
    printTreeRecursively(tree.rootNode());

    // Iterate through all the nodes and call init() if it is an Action_B
    // for (auto &node : tree.nodes)
    // {
    //     // BT Action nodes.
    //     if (auto ExecuteUperbodyMotion_node = dynamic_cast<ExecuteUperbodyMotion *>(node.get()))
    //     {
    //         ExecuteUperbodyMotion_node->init(&nh, &motion);
    //     }
    //     else if (auto ExecuteNavigation_node = dynamic_cast<ExecuteNavigation *>(node.get()))
    //     {
    //         ExecuteNavigation_node->init(&nh, &navigation);
    //     }
    // }

    const bool LOOP = ( argc == 2 && strcmp( argv[1], "loop") == 0);

    do
    {
        NodeStatus status = NodeStatus::RUNNING;
        // Keep on ticking until you get either a SUCCESS or FAILURE state
        while( status == NodeStatus::RUNNING)
        {
            status = tree.tickRoot();
        }
    }
    while(LOOP);

    return 0;
}