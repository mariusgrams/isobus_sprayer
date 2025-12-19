#include "section_control_implement_sim.hpp"
#include "isobus/isobus/can_constants.hpp"
#include "isobus/isobus/isobus_standard_data_description_indices.hpp"
#include "isobus/utility/system_timing.hpp"
#include "isobus/utility/to_string.hpp"

#include <cassert>

SectionControlImplementSimulator::SectionControlImplementSimulator()
{
}

std::uint8_t SectionControlImplementSimulator::get_number_of_sections() const
{
	assert(sectionSwitchStates.size() == sectionSetpointStates.size());
	return static_cast<std::uint8_t>(sectionSwitchStates.size());
}

bool SectionControlImplementSimulator::get_section_actual_state(std::uint8_t index) const
{
	if (index >= sectionSetpointStates.size())
	{
		return false;
	}
	// We currently are just simulating here. In a real implement, you would want to read the actual state from the implement.
	if (isAutoMode)
	{
		return sectionSetpointStates.at(index);
	}
	else
	{
		return sectionSwitchStates.at(index);
	}
}

std::uint8_t SectionControlImplementSimulator::get_actual_number_of_sections_on() const
{
	std::uint8_t retVal = 0;
	for (std::uint8_t i = 0; i < get_number_of_sections(); i++)
	{
		if (true == get_section_actual_state(i))
		{
			retVal++;
		}
	}
	return retVal;
}

bool SectionControlImplementSimulator::get_section_setpoint_state(std::uint8_t index) const
{
	if (index >= sectionSetpointStates.size())
	{
		return false;
	}
	return sectionSetpointStates.at(index);
}

void SectionControlImplementSimulator::set_section_switch_state(std::uint8_t index, bool value)
{
	if (index < sectionSwitchStates.size())
	{
		sectionSwitchStates.at(index) = value;
	}
}

bool SectionControlImplementSimulator::get_section_switch_state(std::uint8_t index) const
{
	if (index >= sectionSwitchStates.size())
	{
		return false;
	}
	return sectionSwitchStates.at(index);
}

std::uint32_t SectionControlImplementSimulator::get_actual_rate() const
{
	bool anySectionOn = get_actual_number_of_sections_on() > 0;
	return targetRate * (anySectionOn ? 1 : 0);
}

std::uint32_t SectionControlImplementSimulator::get_target_rate() const
{
	return targetRate;
}

bool SectionControlImplementSimulator::get_setpoint_work_state() const
{
	return setpointWorkState;
}

void SectionControlImplementSimulator::set_is_mode_auto(bool isAuto)
{
	isAutoMode = isAuto;
}

bool SectionControlImplementSimulator::get_is_mode_auto() const
{
	return isAutoMode;
}

std::uint32_t SectionControlImplementSimulator::get_prescription_control_state() const
{
	return static_cast<std::uint32_t>(get_is_mode_auto());
}

std::uint32_t SectionControlImplementSimulator::get_section_control_state() const
{
	return static_cast<std::uint32_t>(get_is_mode_auto());
}

bool SectionControlImplementSimulator::initialize(std::shared_ptr<isobus::DeviceDescriptorObjectPool> ddop)
{
	if (!ddop)
	{
		return false;
	}

	// Count sections by looking for Section device elements in the DDOP
	std::uint8_t sectionCount = 0;
	for (std::uint16_t i = 0; i < ddop->size(); i++)
	{
		auto obj = ddop->get_object_by_index(i);
		if (obj && obj->get_object_type() == isobus::task_controller_object::ObjectTypes::DeviceElement)
		{
			auto deviceElement = std::static_pointer_cast<isobus::task_controller_object::DeviceElementObject>(obj);
			if (deviceElement->get_type() == isobus::task_controller_object::DeviceElementObject::Type::Section)
			{
				sectionCount++;
			}
		}
	}

	// Initialize section states based on detected section count
	if (sectionCount > 0 && sectionCount <= MAX_NUMBER_SECTIONS_SUPPORTED)
	{
		sectionSetpointStates.resize(sectionCount, false);
		sectionSwitchStates.resize(sectionCount, false);
		return true;
	}

	return false;
}

bool SectionControlImplementSimulator::request_value_command_callback(std::uint16_t,
                                                                      std::uint16_t DDI,
                                                                      std::int32_t &value,
                                                                      void *parentPointer)
{
	if (nullptr != parentPointer)
	{
		auto sim = reinterpret_cast<SectionControlImplementSimulator *>(parentPointer);
		switch (DDI)
		{
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::MaximumCountContent):
			{
				value = 200000; // Arbitrary values... not sure what is a realistic count
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCountContent):
			{
				value = 150000;
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SectionControlState):
			{
				value = sim->get_section_control_state();
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::PrescriptionControlState):
			{
				value = sim->get_prescription_control_state();
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState1_16):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState17_32):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState33_48):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState49_64):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState65_80):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState81_96):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState97_112):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState113_128):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState129_144):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState145_160):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState161_176):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState177_192):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState193_208):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState209_224):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState225_240):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState241_256):
			{
				std::uint8_t sectionIndexOffset = NUMBER_SECTIONS_PER_CONDENSED_MESSAGE * static_cast<std::uint8_t>(DDI - static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState1_16));
				value = 0;

				for (std::uint_fast8_t i = 0; i < NUMBER_SECTIONS_PER_CONDENSED_MESSAGE; i++)
				{
					if ((i + sectionIndexOffset) < sim->get_number_of_sections())
					{
						bool sectionState = sim->get_section_actual_state(i + sectionIndexOffset);
						value |= static_cast<std::uint8_t>(sectionState) << (2 * i);
					}
					else
					{
						value |= (static_cast<std::uint32_t>(0x03) << (2 * i));
					}
				}
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualVolumePerAreaApplicationRate):
			{
				value = sim->get_actual_rate();
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualWorkState):
			{
				value = sim->get_actual_number_of_sections_on() > 0 ? 1 : 0;
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetX):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetY):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::RequestDefaultProcessData):
			{
				value = 0;
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualWorkingWidth):
			{
				value = BOOM_WIDTH;
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState1_16):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState17_32):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState33_48):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState49_64):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState65_80):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState81_96):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState97_112):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState113_128):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState129_144):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState145_160):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState161_176):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState177_192):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState193_208):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState209_224):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState225_240):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState241_256):
			{
				auto sectionIndexOffset = static_cast<std::uint8_t>(NUMBER_SECTIONS_PER_CONDENSED_MESSAGE * (DDI - static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState1_16)));

				value = 0;
				for (std::uint_fast8_t i = 0; i < NUMBER_SECTIONS_PER_CONDENSED_MESSAGE; i++)
				{
					if ((i + sectionIndexOffset) < sim->get_number_of_sections())
					{
						std::uint8_t sectionState = sim->get_section_setpoint_state(i + sectionIndexOffset);
						value |= sectionState << (2 * i);
					}
					else
					{
						value |= (static_cast<std::uint32_t>(0x03) << (2 * i));
					}
				}
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointVolumePerAreaApplicationRate):
			{
				value = sim->get_target_rate();
			}
			break;

			default:
			{
				value = 0;
			}
			break;
		}
	}
	// The actual use of the return value here is for the TC client to know if it needs to keep calling more callbacks to search
	// for one that can satisfy the element number + DDI combination it needs.
	// But in the example this is the only value command callback, so we always want to return true.
	return true;
}

bool SectionControlImplementSimulator::value_command_callback(std::uint16_t,
                                                              std::uint16_t DDI,
                                                              std::int32_t processVariableValue,
                                                              void *parentPointer)
{
	if (nullptr != parentPointer)
	{
		auto sim = reinterpret_cast<SectionControlImplementSimulator *>(parentPointer);
		switch (DDI)
		{
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState1_16):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState17_32):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState33_48):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState49_64):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState65_80):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState81_96):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState97_112):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState113_128):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState129_144):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState145_160):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState161_176):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState177_192):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState193_208):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState209_224):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState225_240):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState241_256):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState1_16):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState17_32):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState33_48):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState49_64):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState65_80):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState81_96):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState97_112):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState113_128):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState129_144):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState145_160):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState161_176):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState177_192):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState193_208):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState209_224):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState225_240):
			// case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState241_256):
			{
				auto sectionIndexOffset = static_cast<std::uint8_t>(NUMBER_SECTIONS_PER_CONDENSED_MESSAGE * (DDI - static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState1_16)));
				//auto sectionIndexOffset = static_cast<std::uint8_t>(NUMBER_SECTIONS_PER_CONDENSED_MESSAGE * (DDI - static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState1_16)));

				for (std::uint_fast8_t i = 0; i < NUMBER_SECTIONS_PER_CONDENSED_MESSAGE; i++)
				{
					if ((i + sectionIndexOffset) < sim->get_number_of_sections())
					{
						bool sectionState = (0x01 == (processVariableValue >> (2 * i) & 0x03));
						sim->sectionSetpointStates.at(sectionIndexOffset + i) = sectionState;
					}
					else
					{
						break;
					}
				}
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointVolumePerAreaApplicationRate):
			{
				sim->targetRate = processVariableValue;
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointWorkState):
			{
				sim->setpointWorkState = (0x01 == processVariableValue);
			}
			break;

			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::PrescriptionControlState):
			case static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SectionControlState):
			{
				sim->set_is_mode_auto(processVariableValue);
			}
			break;

			default:
				break;
		}
	}
	// The actual use of the return value here is for the TC client to know if it needs to keep calling more callbacks to search
	// for one that can satisfy the element number + DDI combination it needs.
	// But in the example this is the only value command callback, so we always want to return true.
	return true;
}
