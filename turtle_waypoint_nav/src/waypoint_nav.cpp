#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"

class WaypointNav : public rclcpp::Node
{
public:
  //constructor
  WaypointNav() : Node("waypoint_nav"){ // string gives a name to the node (initialize)

    RCLCPP_INFO(this->get_logger(), "Turtle is awake!"); //parameters are logger, message string
    pose_subscriber_ = this->create_subscription<turtlesim::msg::Pose>("turtle1/pose", 10, std::bind(&WaypointNav::poseCallback, this, std::placeholders::_1));
    velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);   }
  //subsctiption member 
private:
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber_;

  void poseCallback(const turtlesim::msg::Pose::SharedPtr msg){
    RCLCPP_INFO(this->get_logger(), "x: %f, y: %f", msg->x, msg->y);

    auto velocity_msg = geometry_msgs::msg::Twist();
    velocity_msg.linear.x = 1.0;
    velocity_msg.linear.y = 0;
    velocity_msg.linear.z = 0;
    velocity_publisher_ ->publish(velocity_msg);

  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;

};


int main(int argc, char ** argv)
{

  rclcpp::init(argc, argv);
  auto node = std::make_shared<WaypointNav>(); //auto tells compiler to find type 
  rclcpp::spin(node); //activates the node
  rclcpp::shutdown();
  return 0;
}
