#include "enocean.h"
#include "esphome/core/log.h"
#include <enocean.h>
#include "binary_sensor/enocean_binary_sensor.h"
#include "esphome/components/enocean/sensor/enocean_sensor.h"

#define ENOCEAN_HEADER 4
#define ENOCEAN_MAX_DATA 40

namespace esphome::enocean
{

  static const char *const TAG = "enocean";
  static const uint8_t ENOCEAN_PACKET_LENGTH = 8;

  void Enocean::setup() { ESP_LOGD(TAG, "setup called:"); }

  void Enocean::update() {}

  void Enocean::loop()
  {
    while (this->available())
    {
      uint8_t c;
      this->read_byte(&c);

      if (c == 0x55)
      {
        ESP_LOGD(TAG, "magic byte:");
        ESP_LOGD(TAG, "%d", enocean_binary_sensor_.size());
        do
        {
          yield();
        } while (this->available() < ENOCEAN_HEADER + 1);

        Packet packet;

        this->read_array(packet.getHeader(), ENOCEAN_HEADER);
        int const CRC8H{this->read()};

        if (packet.checkHeaderCRC8(CRC8H))
        {
          ESP_LOGD(TAG, "Header CRC8 is valid.");
        }
        else
        {
          ESP_LOGD(TAG, "Header CRC8 is invalid.");
          return;
        }
        byte waitCounter;
        byte available;
        do
        {
          yield();
          available = this->available();

        } while (available < packet.payloadLength + 1 || waitCounter++ > 50);

        if (packet.payloadLength + 1 <= available)
        {
          Serial.println("read data");

          this->read_array(packet.enocean_data, packet.dataLength);
          this->read_array(packet.enocean_optional, packet.optionalLength);
          byte const CRC8D = this->read();

          if (packet.checkDataCRC8(CRC8D))
          {
            ESP_LOGD(TAG, "Data CRC8 is valid.");
          }
          else
          {
            ESP_LOGD(TAG, "Data CRC8 is invalid.");
            return;
          };
          // if sensor is configured
          for (auto sensor : enocean_sensor_)
          {
            if (strcmp(sensor->get_type().c_str(), "last_rssi") == 0)
            {
              sensor->publish_state(packet.getRssi());
            }
          }
          packet.handleTelegram();
        }

        ESP_LOGD(TAG, "Packet sender: %d %d %d %d", packet.getSenderAddress()[0], packet.getSenderAddress()[1],
                 packet.getSenderAddress()[2], packet.getSenderAddress()[3]);

        for (auto sensor : enocean_binary_sensor_)
        {
          ESP_LOGD(TAG, "packet type: %s, sender type: %s", packet.getType().c_str(), sensor->get_type().c_str());

          if (0 == memcmp(sensor->get_address(), packet.getSenderAddress(), 4))
          {
            if (packet.getType() == "four_button_remote_off")
            {
              for (auto sensor : enocean_binary_sensor_)
              {
                if (0 == memcmp(sensor->get_address(), packet.getSenderAddress(), 4))
                {
                  bool const state = packet.getState() == "on";
                  sensor->publish_state(state);
                }
              }
              break;
            }
            else if (packet.getType() == "Unknown")
            {
              bool const state = packet.getState() == "on";
              sensor->publish_state(state);
              break;
            }
            else if (strcmp(sensor->get_type().c_str(), packet.getType().c_str()) == 0)
            {
              bool const state = packet.getState() == "on";
              sensor->publish_state(state);
              break;
            }
            else if (packet.getType() == "window_handle")
            {
              bool const state = packet.getState() == "on";
              sensor->publish_state(state);
              break;
            }
            else
            {
              // bool const state = packet.getState() == "on";
              // sensor->publish_state(state);
            }
          }
        }

        //         ESP_LOGD(TAG, "Found sensor %s", sensor->get_name());

        //         // for (auto sensor : enocean_binary_sensor_) {
        //         //   if (0 == memcmp(sensor->get_address(), packet.getSenderAddress(), 4)) {
        //     }
        //   }
        //   break;
        // } else {
        //   bool const state = packet.getState() == "on";
        //   ESP_LOGD(TAG, "no compatible sensor found just on");
        //   sensor->publish_state(state);
        //   break;
        // }
        //   }
        // }
      } // magic byte 0x55
    } // whileserial available
  } // function
} // namespace esphome::enocean
