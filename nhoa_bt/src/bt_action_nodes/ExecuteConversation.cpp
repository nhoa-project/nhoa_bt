#include <ExecuteConversation.h>

// ROS INCLUDES
#include <ros/ros.h>

// STD INCLUDES
#include <cstdlib>

// =====================================

BT::NodeStatus ExecuteConversation::tick()
{   
  std::string input;
  bool waiting = true;
  bool success = false;
  while (waiting) 
  {
      std::cout << name() << ": Select 's' (start) or 'p'(pause)" << std::endl;
      std::cin >> input;
      if (input.compare("s") == 0) 
      { 
        // Get the Blackboard input arguments.
        auto conversation_mode  = getInput<std::string>("_conversation_mode");
        auto iteration          = getInput<std::size_t>("_iteration");

        // =======
        if((conversation_mode.value().empty()))
        {
          throw BT::RuntimeError("error reading port [conversation_mode]:", conversation_mode.error());
        }
        // --------
        else if((conversation_mode.value().compare("agenda") == 0) )
        {
          if(!(executeAgenda(iteration.value())))
          {
            std::cout << "ERROR! Not able to execute Agenda conversation." << std::endl;
            success = false;
          }
          else
          {
            std::cout << "SUCCESS! Agenda conversation executed." << std::endl;
            success = true;
          }
        }
        // --------
        else if((conversation_mode.value().compare("agenda_recall") == 0) )
        {
          if(!(executeAgendaRecall(iteration.value())))
          {
              std::cout << "ERROR! Not able to execute Agenda Recall." << std::endl;
              success = false;
          }
          else
          {
            std::cout << "SUCCESS! Agenda Recall executed." << std::endl;
            success = true;
          }
        }
        // --------
        else if(conversation_mode.value().compare("chit_chat") == 0)
        {
          if(!(executeChitChat(iteration.value())))
          {
              std::cout << "ERROR! Not able to start Chit Chat." << std::endl;
              success = false;
          }
          else
          {
              std::cout << "SUCCESS! Starting Chit Chat conversation is reached." << std::endl;
              success = true;
          }
        }
        // --------
        else if((conversation_mode.value().compare("questionnaire") == 0 ) )
        {
          if(!(executeQuestionnaire(iteration.value())))
          {
            std::cout << "ERROR! Not able to execute Questionnaire." << std::endl;
            success = false;
          }
          else
          {
            std::cout << "SUCCESS! Questionnaire executed." << std::endl;
            success = true;
          }
        }
        // --------
        else if((conversation_mode.value().compare("questionnaire_fb") == 0) )
        {
          if(!(executeQuestionnaireFB(iteration.value())))
          {
              std::cout << "ERROR! Not able to execute Questionnaire Feedback." << std::endl;
              success = false;
          }
          else
          {
            std::cout << "SUCCESS! Questionnaire Feedback executed." << std::endl;
            success = true;
          }
        }
        // --------
        else if(conversation_mode.value().compare("questionnaire_start") == 0)
        {
          if(!(startQuestionnaire(iteration.value())))
          {
              std::cout << "ERROR! Not able to start Questionnaire." << std::endl;
              success = false;
          }
          else
          {
              std::cout << "SUCCESS! Starting Questionnaire is reached." << std::endl;
              success = true;
          }
        }
        waiting = false;
      } 
  }

  std::cout << "Asyncronous process finished!"<<std::endl;
  cleanup(false);
  if(success)
  {
    return BT::NodeStatus::SUCCESS;
  }
  else
  {
    return BT::NodeStatus::FAILURE;
  }
}


void ExecuteConversation::cleanup(bool halted)
{
    std::cout << "cleaning up" << std::endl;
    if(halted)
    {
        std::cout << name() <<": cleaning up after an halt()\n" << std::endl;
    }
    else{
        std::cout << name() <<": cleaning up after SUCCESS\n" << std::endl;
    }
}

void ExecuteConversation::halt(){

    std::cout << name() <<": Halted." << std::endl;
    cleanup(true);
    // Do not forget to call this at the end.
    BT::CoroActionNode::halt();

}

// ####################
// Additional functions.

bool ExecuteConversation::executeAgenda(const std::size_t  &iteration)
{
  if(iteration < agenda_cmds.size())
  {
    setOutput("iteration_", iteration + 1);  
    setOutput("voice_cmd_", agenda_cmds[iteration]);

    success_ = true;
  }
  else
  {
    success_ = false;
  }
  return success_;
}

bool ExecuteConversation::executeAgendaRecall(const std::size_t  &iteration)
{
  if(iteration < agenda_recall_cmds.size())
  {
    setOutput("iteration_", iteration + 1);  
    setOutput("voice_cmd_", agenda_recall_cmds[iteration]);
    if(iteration == 1)
    {
      setOutput("motion_name_", "wave");
    }

    success_ = true;
  }
  else
  {
    success_ = false;
  }
  return success_;
}

bool ExecuteConversation::executeChitChat(const std::size_t &iteration)
{
  if (iteration < 2)
  {
    setOutput("iteration_", iteration + 1);  
    setOutput("voice_cmd_", chit_chat_cmds[iteration]);

    success_ = true;
  }
  else if(iteration == 2)
  {
    std::string input;
    bool waiting = true;
    while (waiting)
    {
      std::cout << "Select 'y' (yes) or 'n'(no)." << std::endl;
      std::cin >> input;
      if (input.compare("y") == 0) 
      { 
        setOutput("iteration_", iteration + 1);  
        setOutput("voice_cmd_", chit_chat_cmds[iteration]);
        waiting = false;
      } 
      else if (input.compare("n") == 0) 
      {
        setOutput("iteration_", iteration + 1);  
        setOutput("voice_cmd_", chit_chat_cmds[iteration+1]);
        waiting = false;
      }
    }

    success_ = true;
  }
  else
  { 
    success_ = false;
  }

  return success_;
}

bool ExecuteConversation::executeQuestionnaire(const std::size_t  &iteration)
{
  if(iteration < questionnaire_cmds.size())
  {
    setOutput("iteration_", iteration + 1);  
    setOutput("voice_cmd_", questionnaire_cmds[iteration]);

    success_ = true;
  }
  else
  {
    success_ = false;
  }
  return success_;
}


bool ExecuteConversation::executeQuestionnaireFB(const std::size_t  &iteration)
{
  if(iteration < questionnaire_fb_cmds.size())
  {
    setOutput("iteration_", iteration + 1);  
    setOutput("voice_cmd_", questionnaire_fb_cmds[iteration]);

    success_ = true;
  }
  else
  {
    success_ = false;
  }
  return success_;
}

bool ExecuteConversation::startQuestionnaire(const std::size_t &iteration)
{
  if (iteration == 0)
  {
    setOutput("iteration_", iteration + 1);  
    setOutput("voice_cmd_", questionnaire_start_cmds[iteration]);

    success_ = true;
  }
  else if(iteration == 1)
  {
    std::string input;
    bool waiting = true;
    while (waiting)
    {
      std::cout << "Select 'y' (yes) or 'n'(no)." << std::endl;
      std::cin >> input;
      if (input.compare("y") == 0) 
      { 
        setOutput("iteration_", iteration + 1);  
        setOutput("voice_cmd_", questionnaire_start_cmds[iteration]);
        setOutput("questionnaire_requested_", "true");
        waiting = false;
      } 
      else if (input.compare("n") == 0) 
      {
        setOutput("iteration_", iteration + 1);
        setOutput("voice_cmd_", questionnaire_start_cmds[iteration+1]);
        setOutput("questionnaire_requested_", "false");
        waiting = false;
      }
    }

    success_ = true;
  }
  else
  { 
    success_ = false;
  }

  return success_;
}
