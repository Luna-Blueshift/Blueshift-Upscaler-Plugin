// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Subsystem/BlueshiftUpscalerSubsystem.h"

UBlueshiftUpscalerSubsystem* UBlueshiftUpscalerSubsystem::Get()
{
	return GEngine != nullptr ? GEngine->GetEngineSubsystem<UBlueshiftUpscalerSubsystem>() : nullptr;
}