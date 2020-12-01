#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <algorithm>
#include <cassert>
#include <tracking_tester/optional_bbox_msg.h>
#include <tracking_tester/frame_msg.h>
#include <cmath>

ros::Publisher bbox_pub;

void updateTester(const tracking_tester::frame_msg& frame)
{
    // uncomment to simulate a slow algorithm
    // usleep(3e5);
    unsigned num = frame.number;
    tracking_tester::optional_bbox_msg message;
    static unsigned calls{};
    message.bbox.width = 70;
    message.bbox.height = 50;
    message.bbox.x = frame.image.width / 2 - message.bbox.width / 2 + 25 * std::sin(calls / 30.0);
    message.bbox.y = frame.image.height / 2 - message.bbox.height / 2 + 25 * std::cos(calls / 30.0);
    message.timestamp = ros::Time::now();
    message.valid = true;
    message.number = num;

    bbox_pub.publish(message);
    calls++;
}

void fetchData(const tracking_tester::frame_msg& frame)
{
    updateTester(frame);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "example_tracker");
    ROS_INFO("Initialized!");
    ros::NodeHandle nh;

    auto frame_sub = nh.subscribe("tracking_tester/frame", 1, fetchData);
    bbox_pub = nh.advertise<tracking_tester::optional_bbox_msg>("tracking_tester/bbox", 1, false);
    ros::spin();
}
