from esphome.components import binary_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ADDRESS, CONF_TYPE

from .. import enocean_ns, CONF_ENOCEAN_ID, Enocean

DEPENDENCIES = ["enocean"]
CODEOWNERS = ["@jesserockz"]

EnOceanBinarySensor = enocean_ns.class_(
    "EnOceanBinarySensor", binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(EnOceanBinarySensor)
    .extend(
        {
            cv.GenerateID(CONF_ENOCEAN_ID): cv.use_id(Enocean),
            cv.Required(CONF_ADDRESS): cv.ensure_list(),
            cv.Optional(CONF_TYPE): cv.string,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):

    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    parent = await cg.get_variable(config[CONF_ENOCEAN_ID])
    cg.add(var.set_enocean_parent(parent))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(parent.set_binary_sensor(var))
    if CONF_TYPE in config:
        cg.add(var.set_type(config[CONF_TYPE]))
