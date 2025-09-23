// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

#include "BlueshiftUpscalerMode.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "Upscaler (Blueshift)"))
enum class EBlueshiftUpscalerMode : uint8
{
	None	UMETA(DisplayName = "None"),
	DLSS	UMETA(DisplayName = "DLSS"),
	FSR		UMETA(DisplayName = "FSR"),
	XeSS	UMETA(DisplayName = "XeSS")
};