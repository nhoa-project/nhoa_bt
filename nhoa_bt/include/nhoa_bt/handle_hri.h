#ifndef __handle_hri_H_INCLUDED__
#define __handle_hri_H_INCLUDED__

// C++ standard headers
#include <exception>
#include <string>
#include "std_msgs/Float32.h"

// Boost headers
#include <boost/shared_ptr.hpp>

// ROS headers
#include <ros/ros.h>

// PAL includes
#include <hri_msgs/IdsList.h>
#include <hri_msgs/IdsMatch.h>
#include <hri_msgs/EngagementLevel.h>
#include <hri_msgs/FacialLandmarks.h>

// C++ standard headers
#include <cstdlib>
#include <mutex>
#include <thread>

/* This class encapsulates the HRI functionalities. */

class handle_hri
{
private:
    //vars
    // ROS stuff
    ros::NodeHandle     nh_;

    // HRI stuff.
    // - ROS subscribers.
    ros::Subscriber     candidate_matches_sub_; 
    ros::Subscriber     engagement_level_sub_;
    ros::Subscriber     smile_score_sub_;       // UOC
    ros::Subscriber     tracked_bodies_sub_;
    ros::Subscriber     tracked_faces_sub_;  
    ros::Subscriber     tracked_persons_sub_;

    // - Booleans.
    bool                is_body_id_modified_    = false;
    bool                is_face_id_modified_    = false;
    bool                is_person_id_modified_  = false;

    // - Mutexs.
    std::mutex mutex_; 

    // - Thresholds.
    double              match_confidence_threshold_;

    // =======
    //functions
    // Candidate matches callback.
    void candidates_matches_callback(const hri_msgs::IdsMatch&  ids_match);

    // Person engagement level callback.
    void engagement_level_callback(const hri_msgs::EngagementLevel&  engagement);

    // Initializing.
    void init();

    // Matched ID subscribers' loop. It checks the current face/body/person
    // ID and launches the subscribers.
    void matched_id_subscriber_loop();

    // Set engagement level.
    void set_engagement_level(const uint &level);

    // Set matched ID.
    void set_id_matches(const std::string &id,
                        const std::string &id_type);

    // Smile score callback.
    void smile_score_callback(const std_msgs::Float32::ConstPtr&  score);

    // Set ID type.
    std::string set_id_type(const int   &id_type);

    // Tracked persons callback.
    void tracked_bodies_callback(const hri_msgs::IdsList&  ids_list);

    // Tracked persons callback.
    void tracked_faces_callback(const hri_msgs::IdsList&  ids_list);

    // Tracked persons callback.
    void tracked_persons_callback(const hri_msgs::IdsList&  ids_list);


public:
    //vars
    std::string     body_id_;
    std::string     engagement_level_;
    std::string     face_id_;
    std::string     id1_;
    std::string     id2_;
    std::string     id1_type_;
    std::string     id2_type_;
    std::string     person_id_;

    double          match_confidence_   = 0.0;
    double          smile_score_        = 0.0;
    double          smile_score_threshold_;

    // =======
    //functions
    handle_hri(ros::NodeHandle    *nodehandle);
};
#endif // __handle_hri_H_INCLUDED__