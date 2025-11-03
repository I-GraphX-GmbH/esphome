import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["output"]
CODEOWNERS = ["@philippderdiedas"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True

dac6574_ns = cg.esphome_ns.namespace("dac6574")
DAC6574Output = dac6574_ns.class_("DAC6574Output", cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(DAC6574Output),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x4C))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    return var
