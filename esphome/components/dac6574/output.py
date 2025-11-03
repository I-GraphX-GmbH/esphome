import esphome.codegen as cg
from esphome.components import output
import esphome.config_validation as cv
from esphome.const import CONF_CHANNEL, CONF_ID

from . import DAC6574Output, dac6574_ns

DEPENDENCIES = ["dac6574"]

DAC6574Channel = dac6574_ns.class_("DAC6574Channel", output.FloatOutput)
CONF_DAC6574_ID = "dac6574_id"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(DAC6574Channel),
        cv.GenerateID(CONF_DAC6574_ID): cv.use_id(DAC6574Output),
        cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=3),
    }
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_DAC6574_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(paren.register_channel(var))
    await output.register_output(var, config)
    return var
