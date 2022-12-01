FROM gitlab.local.eurecat.org:5050/robotics-automation/robotics-dockers/ros_dev:noetic

# Root permissions necessary.
USER root

# Prepare ROS workspace.
RUN mkdir -p /nhoa_bt_ws
WORKDIR /nhoa_bt_ws
   
RUN apt-get update && \
    apt-get install -y git && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    libzmq3-dev \
    libboost-dev \
    ros-noetic-behaviortree-cpp-v3 \
    ros-noetic-control-msgs \
    ros-noetic-controller-manager-msgs \
    ros-noetic-moveit-ros-planning-interface \
    ros-noetic-controller-manager \
    ros-noetic-move-base-msgs

# Clone necessary packages.
WORKDIR /nhoa_bt_ws/src

# CLone github repos.
# RUN git clone https://github.com/NHoA-project/nhoa_bt.git
# RUN git clone https://github.com/pal-robotics/play_motion.git
# RUN git clone https://github.com/pal-robotics/pal_msgs.git
ADD nhoa ./nhoa
ADD play_motion_msgs ./play_motion_msgs
ADD pal_interaction_msgs ./pal_interaction_msgs/
WORKDIR /nhoa_bt_ws

RUN /bin/bash -c "source /opt/ros/noetic/setup.bash; catkin build"

# Add source to bashrc
RUN echo "" >> ~/.bashrc
RUN echo "## ROS" >> ~/.bashrc
RUN echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
RUN echo "source /nhoa_bt_ws/devel/setup.bash" >> ~/.bashrc