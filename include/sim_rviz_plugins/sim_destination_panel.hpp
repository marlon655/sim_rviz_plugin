#ifndef SIM_RVIZ_PLUGINS__SIM_DESTINATION_PANEL_HPP_
#define SIM_RVIZ_PLUGINS__SIM_DESTINATION_PANEL_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rviz_common/panel.hpp"
#include "std_msgs/msg/int32.hpp"

namespace sim_rviz_plugins
{

class SimDestinationPanel : public rviz_common::Panel
{
  Q_OBJECT

public:
  explicit SimDestinationPanel(QWidget * parent = nullptr);

private Q_SLOTS:
  void publishDestination(int destination_id);

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr destination_pub_;
};

}  // namespace sim_rviz_plugins

#endif  // SIM_RVIZ_PLUGINS__SIM_DESTINATION_PANEL_HPP_
