#include "esphome/core/log.h"
#include "enocean_binary_sensor.h"

namespace esphome {
namespace enocean {

static const char *const TAG = "enocean.binary_sensor";

void EnOceanBinarySensor::setup() {}

void EnOceanBinarySensor::dump_config() { ESP_LOGCONFIG(TAG, "EnOcean Binary Sensor"); }

}  // namespace enocean
}  // namespace esphome
