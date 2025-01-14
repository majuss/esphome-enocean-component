#pragma once

#include "esphome/core/component.h"
#include "esphome/components/enocean/enocean.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome::enocean {

class EnOceanBinarySensor : public binary_sensor::BinarySensor, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_sensor_id(uint8_t sensor_id) { this->sensor_id_ = sensor_id; }

  void set_enocean_parent(Enocean *parent) { this->parent_ = parent; }

  void set_address(std::array<uint8_t, 4> const &addr) { memcpy(address_, addr.data(), 4); }
  void set_type(String type) { this->type_ = type; }

  uint8_t const *const get_address() const { return address_; }

  String get_type() const { return type_; }

 protected:
  Enocean *parent_{nullptr};
  String type_{};
  uint8_t sensor_id_{0};
  uint8_t address_[4]{0};
};

}  // namespace esphome::enocean
