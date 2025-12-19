//================================================================================================
/// @file sprayer.hpp
///
/// @brief This is the definition of an example sprayer application
/// @author Adrian Del Grosso
///
/// @copyright 2023 The Open-Agriculture Developers
//================================================================================================
#ifndef SPRAYER_HPP
#define SPRAYER_HPP

#include "isobus/isobus/isobus_diagnostic_protocol.hpp"
#include "vt_application.hpp"

class Sprayer
{
public:
	Sprayer() = default;

	bool initialize();

	void terminate();

	void update();

private:
	std::unique_ptr<SprayerVtApplication> VTApplication = nullptr;
	std::unique_ptr<isobus::DiagnosticProtocol> diagnosticProtocol = nullptr;
};

#endif // SPRAYER_HPP