#include <mpc-rbt-solution/Receiver.hpp>

void Receiver::Node::run()
{
  while (errno != EINTR) {
    RCLCPP_INFO(logger, "Waiting for data ...");
    Socket::IPFrame frame{};
    if (receive(frame)) {
      RCLCPP_INFO(logger, "Received data from host: '%s:%d'", frame.address.c_str(), frame.port);

      callback(frame);

    } else {
      RCLCPP_WARN(logger, "Failed to receive data.");
    }
  }
}

void Receiver::Node::onDataReceived(const Socket::IPFrame & frame)
{
  try {
    // Deserializace přijatých dat
    json j = json::parse(frame.data);
    data = Utils::jsonToMessage(j);

    // Logování přijatých dat
    RCLCPP_INFO(logger, "Received data:");
    RCLCPP_INFO(logger, "\t x: %.2f\n\t y: %.2f\n\t z: %.2f\n\t timestamp: %ld", data.x, data.y, data.z, data.timestamp);

  } catch (const std::exception & e) {
    RCLCPP_ERROR(logger, "Failed to parse received data: %s", e.what());
  }
}

