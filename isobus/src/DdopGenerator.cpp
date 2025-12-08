#include "isobus/isobus/DdopGenerator.h"
#include "isobus/isobus/isobus_standard_data_description_indices.hpp"
#include "isobus/isobus/can_constants.hpp"
#include <string>

static constexpr std::uint8_t NUMBER_SECTIONS_PER_CONDENSED_MESSAGE = 16;

DdopGenerator::DdopGenerator(std::vector<Boom> booms,
                isobus::NAME clientName,
                std::string deviceName, 
                std::string deviceVersion)
  :  mBooms(booms)
{
  mDdop = std::make_shared<isobus::DeviceDescriptorObjectPool>(3);
  std::uint16_t elementCounter = 0;

	// English, decimal point, 12 hour time, ddmmyyyy, all units imperial
	constexpr std::array<std::uint8_t, 7> localizationData = { 'e', 'n', 0b01010000, 0x00, 0b01010101, 0b01010101, 0xFF };

	// Set up device and device element
	mDdop->add_device(deviceName, "1.0.0", "123", deviceVersion, localizationData, std::vector<std::uint8_t>(), clientName.get_full_name());
	mDdop->add_device_element("Sprayer", elementCounter, 0, isobus::task_controller_object::DeviceElementObject::Type::Device, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainDeviceElement));
	mDdop->add_device_process_data("Actual Work State", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualWorkState), isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::DeviceActualWorkState));
	mDdop->add_device_process_data("Request Default PD", static_cast<std::uint16_t>(ImplementDDOPObjectIDs::RequestDefaultProcessData), isobus::NULL_OBJECT_ID, 0, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::Total), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::RequestDefaultProcessData));
	mDdop->add_device_process_data("Total Time", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::EffectiveTotalTime), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::TimePresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::Total), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::DeviceTotalTime));
	elementCounter++; // Increment element number. Needs to be unique for each element.

	// Set up connector element
	mDdop->add_device_element("Connector", elementCounter, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainDeviceElement), isobus::task_controller_object::DeviceElementObject::Type::Connector, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Connector));
	mDdop->add_device_process_data("Connector X", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetX), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), 0, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ConnectorXOffset));
	mDdop->add_device_process_data("Connector Y", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetY), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), 0, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ConnectorYOffset));
	mDdop->add_device_property("Type", 9, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ConnectorType), isobus::NULL_OBJECT_ID, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ConnectorType));
	elementCounter++; // Increment element number. Needs to be unique for each element.

	// Set up Boom element
  for (const auto& boom : mBooms)
  {
    mDdop->add_device_element("Boom", elementCounter, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainDeviceElement), isobus::task_controller_object::DeviceElementObject::Type::Function, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainBoom));
    mDdop->add_device_property("Offset X", 0, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetX), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BoomXOffset));
    mDdop->add_device_property("Offset Y", 0, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetY), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BoomYOffset));
    mDdop->add_device_property("Offset Z", 0, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetZ), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BoomZOffset));
    mDdop->add_device_process_data("Actual Working Width", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualWorkingWidth), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::LongWidthPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualWorkingWidth));
    mDdop->add_device_process_data("Area Total", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::TotalArea), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::AreaPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::Total), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::AreaTotal));
    mDdop->add_device_process_data("Setpoint Work State", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointWorkState), isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::SetpointWorkState));
    
    if (boom.supportsSectionControl)
    {
      mDdop->add_device_process_data("Section Control State", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SectionControlState), isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::TimeInterval), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::SectionControlState));
      if (boom.numberOfSections > 0)
      {
        std::uint16_t sectionCounter = 0;
        while (sectionCounter < boom.numberOfSections)
        {
          mDdop->add_device_process_data("Actual Work State 1-16", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCondensedWorkState1_16) + (sectionCounter / NUMBER_SECTIONS_PER_CONDENSED_MESSAGE), isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualCondensedWorkingState1To16) + (sectionCounter / NUMBER_SECTIONS_PER_CONDENSED_MESSAGE));
          mDdop->add_device_process_data("Setpoint Work State 1-16", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointCondensedWorkState1_16) + (sectionCounter / NUMBER_SECTIONS_PER_CONDENSED_MESSAGE), isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::SetpointCondensedWorkingState1To16) + (sectionCounter / NUMBER_SECTIONS_PER_CONDENSED_MESSAGE));
          sectionCounter += NUMBER_SECTIONS_PER_CONDENSED_MESSAGE;
        }  
      }
    }
    elementCounter++; // Increment element number. Needs to be unique for each element.

    // Set up sections for section control
    const std::int32_t SECTION_WIDTH = (boom.boomWidthMm / boom.numberOfSections);
    for (std::uint_fast8_t i = 0; i < boom.numberOfSections; i++)
    {
      std::int32_t individualSectionWidth = boom.boomWidthMm / boom.numberOfSections;
      std::int32_t sectionOffsetY = ((-static_cast<std::int32_t>(boom.boomWidthMm)) / 2) + (static_cast<std::int32_t>(i) * SECTION_WIDTH) + (SECTION_WIDTH / 2);
      
      mDdop->add_device_element("Section " + std::to_string(static_cast<int>(i)), elementCounter, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainBoom), isobus::task_controller_object::DeviceElementObject::Type::Section, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1) + i);
      mDdop->add_device_property("Offset X", -20, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetX), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1XOffset) + i);
      mDdop->add_device_property("Offset Y", sectionOffsetY, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::DeviceElementOffsetY), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1YOffset) + i);
      mDdop->add_device_property("Width", individualSectionWidth, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualWorkingWidth), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1Width) + i);
      auto section = std::static_pointer_cast<isobus::task_controller_object::DeviceElementObject>(mDdop->get_object_by_id(i + static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1)));
      section->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1YOffset) + i);
      section->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1XOffset) + i);
      section->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Section1Width) + i);
      elementCounter++; // Increment element number. Needs to be unique for each element, and each section is its own element.
    }
  }

	// Set up bin/tank element
	mDdop->add_device_element("Product", elementCounter, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainBoom), isobus::task_controller_object::DeviceElementObject::Type::Bin, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::GranularProduct));
	mDdop->add_device_process_data("Bin Capacity", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::MaximumCountContent), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::CountPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::TimeInterval), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BinCapacity));
	mDdop->add_device_process_data("Bin Level", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCountContent), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::CountPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::TimeInterval), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BinLevel));
	mDdop->add_device_process_data("Lifetime Total Count", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::LifetimeApplicationTotalCount), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::CountPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::Total), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::LifetimeApplicationCountTotal));
	mDdop->add_device_process_data("Rx Control State", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::PrescriptionControlState), isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::TimeInterval), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::PrescriptionControlState));
	mDdop->add_device_process_data("Target Rate", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::SetpointVolumePerAreaApplicationRate), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::CountPerAreaPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::Settable), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::TargetRate));
	mDdop->add_device_process_data("Actual Rate", static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualVolumePerAreaApplicationRate), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::CountPerAreaPresentation), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange) | static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::TimeInterval), static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualRate));
	mDdop->add_device_property("Operation Type", 6, static_cast<std::uint16_t>(isobus::DataDescriptionIndex::ActualCulturalPractice), isobus::NULL_OBJECT_ID, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualCulturalPractice));
	elementCounter++; // Increment element number. Needs to be unique for each element.


	// Set up presentations
  mDdop->add_device_value_presentation("mm", 0, 1.0f, 0, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ShortWidthPresentation));
  mDdop->add_device_value_presentation("m", 0, 0.001f, 0, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::LongWidthPresentation));
  mDdop->add_device_value_presentation("m^2", 0, 1.0f, 0, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::AreaPresentation));
  mDdop->add_device_value_presentation("seeds", 0, 1.0f, 0, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::CountPresentation));
  mDdop->add_device_value_presentation("minutes", 0, 1.0f, 1, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::TimePresentation));
  mDdop->add_device_value_presentation("seeds/ha", 0, 1.0f, 0, static_cast<std::uint16_t>(ImplementDDOPObjectIDs::CountPerAreaPresentation));

	// Add child linkages to device elements
	auto sprayer = std::static_pointer_cast<isobus::task_controller_object::DeviceElementObject>(mDdop->get_object_by_id(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainDeviceElement)));
	auto connector = std::static_pointer_cast<isobus::task_controller_object::DeviceElementObject>(mDdop->get_object_by_id(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::Connector)));
	auto boom = std::static_pointer_cast<isobus::task_controller_object::DeviceElementObject>(mDdop->get_object_by_id(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::MainBoom)));
	auto product = std::static_pointer_cast<isobus::task_controller_object::DeviceElementObject>(mDdop->get_object_by_id(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::GranularProduct)));

	if (sprayer)
	{
		sprayer->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::DeviceActualWorkState));
		sprayer->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::RequestDefaultProcessData));
		sprayer->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::DeviceTotalTime));
	}

	if (connector)
	{
		connector->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ConnectorXOffset));
		connector->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ConnectorYOffset));
		connector->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ConnectorType));
	}

	if (boom && !mBooms.empty())
	{
		boom->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BoomXOffset));
		boom->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BoomYOffset));
		boom->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BoomZOffset));
		boom->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualWorkingWidth));
		boom->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::SectionControlState));

		std::uint16_t sectionCounter = 0;
		while (sectionCounter < mBooms[0].numberOfSections)
		{
			boom->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualCondensedWorkingState1To16) + (sectionCounter / NUMBER_SECTIONS_PER_CONDENSED_MESSAGE));
			boom->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::SetpointCondensedWorkingState1To16) + (sectionCounter / NUMBER_SECTIONS_PER_CONDENSED_MESSAGE));
			sectionCounter += NUMBER_SECTIONS_PER_CONDENSED_MESSAGE;
		}
	}

	if (product)
	{
		product->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BinCapacity));
		product->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::BinLevel));
		product->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::LifetimeApplicationCountTotal));
		product->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::PrescriptionControlState));
		product->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualCulturalPractice));
		product->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::TargetRate));
		product->add_reference_to_child_object(static_cast<std::uint16_t>(ImplementDDOPObjectIDs::ActualRate));
	}
}

DdopGenerator::~DdopGenerator() {}

std::shared_ptr<isobus::DeviceDescriptorObjectPool>DdopGenerator::getDdop() const
{
  return mDdop;
}