// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

#include "BlueshiftAAMethod.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "AA Method (Blueshift)"))
enum class EBlueshiftAAMethod : uint8
{
	None	= 0,
	FXAA	= 1,
	TAA		= 2,
	MSAA	= 3,
	TSR		= 4,
	SMAA	= 5,
	MAX		UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EBlueshiftAAMethod, EBlueshiftAAMethod::MAX);