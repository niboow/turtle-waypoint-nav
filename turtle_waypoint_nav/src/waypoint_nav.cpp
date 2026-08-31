#include "rclcpp/rclcpp.hpp"

class WaypointNav : public rclcpp::Node
{
public:
  //constructor
  WaypointNav() : Node("waypoint_nav"){ // string gives a name to the node (initialize)

    RCLCPP_INFO(this->get_logger(), "Turtle is awake!"); //parameters are logger, message string

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
