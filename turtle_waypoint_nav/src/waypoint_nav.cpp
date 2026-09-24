#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <vector>
#include <utility>
#include <cmath>

class WaypointNav : public rclcpp::Node
{
public:
  //constructor
  WaypointNav() : Node("waypoint_nav"){ // string gives a name to the node (initialize)

    RCLCPP_INFO(this->get_logger(), "Turtle is awake!"); //parameters are logger, message string
    pose_subscriber_ = this->create_subscription<turtlesim::msg::Pose>("turtle1/pose", 10, std::bind(&WaypointNav::poseCallback, this, std::placeholders::_1));
    velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);   

    waypoints_.push_back(std::pair{5.0, 5.0}); //squiggle brackets allow c++ to automatically detect the type of pair
    waypoints_.push_back(std::pair{2.4, 7.0});
    waypoints_.push_back(std::pair{8.0, 7.0});
    waypoints_.push_back(std::pair{5.5, 5.5});  // back near turtle's start
  }

private:
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
  std::vector<std::pair<double,double>> waypoints_;
  size_t current_waypoint_index_ = 0; //keeping track of waypoint 


  void poseCallback(const turtlesim::msg::Pose::SharedPtr msg){

    double goal_x = waypoints_[current_waypoint_index_].first; //going into the vector, getting first double 
    double goal_y = waypoints_[current_waypoint_index_].second; //second double 

    double dx = goal_x - msg->x;
    double dy = goal_y - msg->y;
    double distance = std::sqrt(dx*dx + dy*dy); //pythoagorean

    if(distance < 0.3){ //the threshold for counting our goal reached
      current_waypoint_index_++; //next waypoint
      if (current_waypoint_index_ >= waypoints_.size()){ //if at the end of the list
        auto stop_msg = geometry_msgs::msg::Twist();
        velocity_publisher_->publish(stop_msg); //sets everything to 0
        RCLCPP_INFO(this->get_logger(), "All waypoints reached!");
        return;
      }
      //recompute against the new goal if end of list was not reached

      //setting new values
      goal_x = waypoints_[current_waypoint_index_].first; 
      goal_y = waypoints_[current_waypoint_index_].second;
      dx = goal_x - msg->x; 
      dy = goal_y - msg->y;
      distance = std::sqrt(dx*dx + dy*dy);
    }

    double angle_to_goal = std::atan2(dy, dx);
    double angle_diff = angle_to_goal - msg->theta;
    while (angle_diff > M_PI) angle_diff -= 2 * M_PI;
    while (angle_diff < -M_PI) angle_diff += 2 * M_PI;

    double linear_speed = 0.3*distance;
    double angular_speed = 0.8 * angle_diff;

    auto velocity_msg = geometry_msgs::msg::Twist();
    velocity_msg.linear.x = linear_speed;
    velocity_msg.angular.z = angular_speed;
    velocity_publisher_ ->publish(velocity_msg);
  }

};


int main(int argc, char ** argv)
{

  rclcpp::init(argc, argv);
  auto node = std::make_shared<WaypointNav>(); //auto tells compiler to find type 
  rclcpp::spin(node); //activates the node
  rclcpp::shutdown();
  return 0;
}
