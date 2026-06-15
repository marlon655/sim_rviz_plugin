#include "sim_rviz_plugins/sim_destination_panel.hpp"

#include <memory>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "pluginlib/class_list_macros.hpp"

namespace sim_rviz_plugins
{

SimDestinationPanel::SimDestinationPanel(QWidget * parent)
: rviz_common::Panel(parent),
  node_(std::make_shared<rclcpp::Node>("sim_destination_panel"))
{
  auto * main_layout = new QVBoxLayout;

  auto * title_label = new QLabel("Destinos");
  title_label->setAlignment(Qt::AlignCenter);
  main_layout->addWidget(title_label);

  auto * button_grid = new QGridLayout;
  constexpr int kDestinationCount = 18;
  constexpr int kColumns = 6;

  for (int destination_id = 1; destination_id <= kDestinationCount; ++destination_id) {
    auto * button = new QPushButton(QString::number(destination_id));
    button->setMinimumHeight(36);

    connect(button, &QPushButton::clicked, this, [this, destination_id]() {
      publishDestination(destination_id);
    });

    const int row = (destination_id - 1) / kColumns;
    const int column = (destination_id - 1) % kColumns;
    button_grid->addWidget(button, row, column);
  }

  main_layout->addLayout(button_grid);
  main_layout->addStretch();
  setLayout(main_layout);

  const auto qos = rclcpp::QoS(rclcpp::KeepLast(1)).reliable().transient_local();
  destination_pub_ = node_->create_publisher<std_msgs::msg::Int32>("/destination", qos);
}

void SimDestinationPanel::publishDestination(int destination_id)
{
  auto msg = std_msgs::msg::Int32();
  msg.data = destination_id;
  destination_pub_->publish(msg);

  RCLCPP_INFO(
    node_->get_logger(),
    "Destino %d publicado em /destination",
    destination_id);
}

}  // namespace sim_rviz_plugins

PLUGINLIB_EXPORT_CLASS(sim_rviz_plugins::SimDestinationPanel, rviz_common::Panel)
