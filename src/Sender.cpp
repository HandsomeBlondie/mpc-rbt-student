#include <mpc-rbt-solution/Sender.hpp>

void Sender::Node::run()
{
  while (errno != EINTR) {
    if ((std::chrono::steady_clock::now() - timer_tick) < timer_period) continue;
    timer_tick = std::chrono::steady_clock::now();

    callback();
  }
}

void Sender::Node::onDataTimerTick()
{
  // Generování dat
  data.x += 1.0;
  data.y += 2.0;
  data.z += 3.0;

  data.timestamp = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());

  // Serializace do JSON
  json j = Utils::messageToJson(data);
  std::string serializedData = j.dump();

  // Nastavení cílového hosta
  Socket::IPFrame frame{
    .port = config.remotePort,
    .address = config.remoteAddress,
    .data = serializedData
  };

  // Logování odesílání
  RCLCPP_INFO(logger, "Sending data to host: '%s:%d'", frame.address.c_str(), frame.port);
  RCLCPP_INFO(logger, "\t x: %.2f\n\t y: %.2f\n\t z: %.2f\n\t timestamp: %ld", data.x, data.y, data.z, data.timestamp);

  // Odeslání
  send(frame);
}

