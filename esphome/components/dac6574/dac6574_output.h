#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace dac6574 {

class DAC6574Output;

class DAC6574Channel : public output::FloatOutput, public Parented<DAC6574Output> {
 public:
  void set_channel(uint8_t channel) { channel_ = channel; }

 protected:
  friend class DAC6574Output;

  const uint16_t full_scale_ = 0x3FF;

  void write_state(float state) override;

  uint8_t channel_;
};

/// DAC6574 float output component.
class DAC6574Output : public Component, public i2c::I2CDevice {
 public:
  DAC6574Output() {}

  void register_channel(DAC6574Channel *channel);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

 protected:
  friend DAC6574Channel;

  bool internal_reference_;

  void set_channel_value_(uint8_t channel, uint16_t value);

  uint8_t min_channel_{0xFF};
  uint8_t max_channel_{0x00};
  uint16_t dac_input_reg_[4] = {
      0,
  };
};

}  // namespace dac6574
}  // namespace esphome
