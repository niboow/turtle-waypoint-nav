#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <vector>
#include <utility>

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
    RCLCPP_INFO(this->get_logger(), "x: %f, y: %f", msg->x, msg->y);

    auto velocity_msg = geometry_msgs::msg::Twist();
    velocity_msg.linear.x = 1.0;
    velocity_msg.linear.y = 0;
    velocity_msg.linear.z = 0;
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
