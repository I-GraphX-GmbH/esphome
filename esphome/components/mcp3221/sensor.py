import esphome.codegen as cg
from esphome.components import i2c, sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_RAW,
    CONF_REFERENCE_VOLTAGE,
    DEVICE_CLASS_VOLTAGE,
    ICON_SCALE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
)

DEPENDENCIES = ["i2c"]

# CONF_CONTINUOUS_MODE = "continuous_mode"


mcp3221_ns = cg.esphome_ns.namespace("mcp3221")
MCP3221Sensor = mcp3221_ns.class_(
    "MCP3221Sensor", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice
)


CONFIG_SCHEMA = (
    sensor.sensor_schema(
        MCP3221Sensor,
        icon=ICON_SCALE,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
        device_class=DEVICE_CLASS_VOLTAGE,
        unit_of_measurement=UNIT_VOLT,
    )
    .extend(
        {
            cv.Optional(CONF_REFERENCE_VOLTAGE, default="3.3V"): cv.voltage,
            cv.Optional(CONF_RAW, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x48))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
