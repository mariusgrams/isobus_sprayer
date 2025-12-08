#ifndef DDOPGENERATOR_H
#define DDOPGENERATOR_H

#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "isobus/isobus/isobus_device_descriptor_object_pool.hpp"

class DdopGenerator
{
public:

  struct Boom
  {
    std::uint16_t targetRateDdi{1};
    std::uint8_t numberOfRateChannels{8};
    bool supportsPrescriptionControl{true};
    std::uint8_t numberOfSections{10};
    bool supportsSectionControl{true};
    std::uint32_t boomWidthMm{15000};
  };

  DdopGenerator(std::vector<Boom> booms,
                isobus::NAME clientName, 
                std::string deviceName, 
                std::string deviceVersion = "1.0.0");
  ~DdopGenerator();

  std::shared_ptr<isobus::DeviceDescriptorObjectPool> getDdop() const;

private:
  static constexpr std::uint16_t MAX_NUMBER_SECTIONS_SUPPORTED = 256;

  enum class ImplementDDOPObjectIDs : std::uint16_t
  {
    Device = 0,
    MainDeviceElement,
    DeviceActualWorkState,
    RequestDefaultProcessData,
    DeviceTotalTime,
    Connector,
    ConnectorXOffset,
    ConnectorYOffset,
    ConnectorType,
    MainBoom,
    ActualWorkState,
    ActualWorkingWidth,
    AreaTotal,
    SetpointWorkState,
    SectionControlState,
    BoomXOffset,
    BoomYOffset,
    BoomZOffset,
    Section1,
    SectionMax = Section1 + (MAX_NUMBER_SECTIONS_SUPPORTED - 1),
    Section1XOffset,
    SectionXOffsetMax = Section1XOffset + (MAX_NUMBER_SECTIONS_SUPPORTED - 1),
    Section1YOffset,
    SectionYOffsetMax = Section1YOffset + (MAX_NUMBER_SECTIONS_SUPPORTED - 1),
    Section1Width,
    SectionWidthMax = Section1Width + (MAX_NUMBER_SECTIONS_SUPPORTED - 1),
    ActualCondensedWorkingState1To16,
    ActualCondensedWorkingState17To32,
    ActualCondensedWorkingState33To48,
    ActualCondensedWorkingState49To64,
    ActualCondensedWorkingState65To80,
    ActualCondensedWorkingState81To96,
    ActualCondensedWorkingState97To112,
    ActualCondensedWorkingState113To128,
    ActualCondensedWorkingState129To144,
    ActualCondensedWorkingState145To160,
    ActualCondensedWorkingState161To176,
    ActualCondensedWorkingState177To192,
    ActualCondensedWorkingState193To208,
    ActualCondensedWorkingState209To224,
    ActualCondensedWorkingState225To240,
    ActualCondensedWorkingState241To256,
    SetpointCondensedWorkingState1To16,
    SetpointCondensedWorkingState17To32,
    SetpointCondensedWorkingState33To48,
    SetpointCondensedWorkingState49To64,
    SetpointCondensedWorkingState65To80,
    SetpointCondensedWorkingState81To96,
    SetpointCondensedWorkingState97To112,
    SetpointCondensedWorkingState113To128,
    SetpointCondensedWorkingState129To144,
    SetpointCondensedWorkingState145To160,
    SetpointCondensedWorkingState161To176,
    SetpointCondensedWorkingState177To192,
    SetpointCondensedWorkingState193To208,
    SetpointCondensedWorkingState209To224,
    SetpointCondensedWorkingState225To240,
    SetpointCondensedWorkingState241To256,
    GranularProduct,
    BinCapacity,
    BinLevel,
    LifetimeApplicationCountTotal,
    PrescriptionControlState,
    ActualCulturalPractice,
    TargetRate,
    ActualRate,
    AreaPresentation,
    TimePresentation,
    ShortWidthPresentation,
    LongWidthPresentation,
    CountPresentation,
    CountPerAreaPresentation
  };

  enum class ObjectTypeId : uint16_t
  {
    Device = 0,
    MainDeviceElement,
    Connector,
    MainBoom,
    AreaPresentation, 
    TimePresentation,
    ShortWidthPresentation,
    LongWidthPresentation,
    CountPresentation,
    CountPerAreaPresentation
  };

  std::vector<Boom> mBooms;
  std::shared_ptr<isobus::DeviceDescriptorObjectPool> mDdop;
};

#endif