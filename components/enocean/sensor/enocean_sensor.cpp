#include "esphome/core/log.h"
#include "enocean_sensor.h"

namespace esphome {
namespace enocean {

static const char *const TAG = "enocean.sensor";

void EnOceanSensor::setup() {}

void EnOceanSensor::dump_config() { ESP_LOGCONFIG(TAG, "EnOcean Sensor"); }

}  // namespace enocean
}  // namespace esphome
