#include "mcp3221_sensor.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mcp3221 {

static const char *const TAG = "mcp3221";

void MCP3221Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Probing MCP3221...");

  if (this->write(nullptr, 0) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
}

float MCP3221Sensor::get_setup_priority() const { return setup_priority::DATA; }

float MCP3221Sensor::sample() {
  uint8_t data[2];
  if (this->read(data, 2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read data from MCP3221");
    this->status_set_warning();
    return NAN;
  }
  this->status_clear_warning();

  uint16_t value = encode_uint16(data[0], data[1]);
  float voltage = value * this->reference_voltage_ / 4096.0f;

  return voltage;
}

void MCP3221Sensor::update() {
  float v = this->sample();
  this->publish_state(v);
}

}  // namespace mcp3221
}  // namespace esphome
