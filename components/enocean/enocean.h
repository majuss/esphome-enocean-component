#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome::enocean {

class EnOceanBinarySensor;

class Enocean : public PollingComponent, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void setup() override;
  void update() override;

  void loop() override;

  void set_binary_sensor(EnOceanBinarySensor *enocean_binary_sensor) {
    enocean_binary_sensor_.push_back(enocean_binary_sensor);
  }

  void set_sensor_id(uint8_t sensor_id) { this->sensor_id_ = sensor_id; }

 protected:
  uint8_t sensor_id_{0};
  std::vector<EnOceanBinarySensor *> enocean_binary_sensor_{};
};

}  // namespace esphome::enocean
