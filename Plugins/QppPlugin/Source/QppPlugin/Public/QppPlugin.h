// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
// 1. Tell Unreal to ignore standard third-party warnings
THIRD_PARTY_INCLUDES_START

// 2. Explicitly disable the MSVC warnings
#pragma warning(push)
#pragma warning(disable: 4800) // Implicit conversion from 'int' to bool
#pragma warning(disable: 4457) // Variable shadowing
#pragma warning(disable: 4668) // Undefined preprocessor macro (This fixes the Eigen/Qpp error!)

// 3. Include the library
#include "../../../ThirdParty/qpp.hpp"

// 4. Restore the warnings and Unreal's strict rules
#pragma warning(pop)
THIRD_PARTY_INCLUDES_END

class FQppPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
