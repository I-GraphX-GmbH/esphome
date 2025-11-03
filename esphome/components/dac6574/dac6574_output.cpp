#include "dac6574_output.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dac6574 {

static const char *const TAG = "dac6574";

static const uint8_t DAC6574_CONTROL_BYTE = 0x10;
static const uint8_t DAC6574_CH_SHIFT = 1;

static const uint8_t DAC6574_BROADCAST = 0x08;

void DAC6574Output::setup() {
  ESP_LOGV(TAG, "Resetting channels");

  const uint8_t reset_value[3] = {DAC6574_BROADCAST, 0x00, 0x00};

  // Reset all channels to 0V
  if (this->write(reset_value, sizeof(reset_value)) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Reset failed");
    this->mark_failed();
    return;
  } else {
    ESP_LOGV(TAG, "Reset succeeded");
  }
}

void DAC6574Output::dump_config() {
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Setting up DAC6574 failed!");
  } else {
    ESP_LOGCONFIG(TAG, "DAC6574 initialised");
  }
}

void DAC6574Output::register_channel(DAC6574Channel *channel) {
  auto c = channel->channel_;
  this->min_channel_ = std::min(this->min_channel_, c);
  this->max_channel_ = std::max(this->max_channel_, c);
  channel->set_parent(this);
  ESP_LOGV(TAG, "Registered channel: %01u", channel->channel_);
}

void DAC6574Output::set_channel_value_(uint8_t channel, uint16_t value) {
  if (this->dac_input_reg_[channel] != value) {
    ESP_LOGV(TAG, "Channel %01u: input_reg=%04u ", channel, value);

    const uint8_t data[3] = {
        static_cast<uint8_t>(DAC6574_CONTROL_BYTE | (static_cast<uint8_t>(channel << DAC6574_CH_SHIFT))),
        static_cast<uint8_t>((value >> 2) & 0xFF),
        static_cast<uint8_t>((value & 0x03) << 6),
    };

    if (this->write(data, sizeof(data)) != i2c::ERROR_OK) {
      this->status_set_warning();
      return;
    }
  }
  this->dac_input_reg_[channel] = value;
  this->status_clear_warning();
}

void DAC6574Channel::write_state(float state) {
  const float input_rounded = roundf(state * this->full_scale_);
  auto input = static_cast<uint16_t>(input_rounded);
  this->parent_->set_channel_value_(this->channel_, input);
}

}  // namespace dac6574
}  // namespace esphome
