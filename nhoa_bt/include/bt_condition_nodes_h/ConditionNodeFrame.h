#ifndef ConditionNodeFrame_H_INCLUDED__
#define ConditionNodeFrame_H_INCLUDED__

#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

class ConditionNodeFrame : public BT::ConditionNode
{
  public:
    ConditionNodeFrame(const std::string& name) :
        BT::ConditionNode(name, {})
    {
    }

    // You must override the virtual function tick()
    BT::NodeStatus tick() override;
};
#endif //ConditionNodeFrame_H_INCLUDED__
