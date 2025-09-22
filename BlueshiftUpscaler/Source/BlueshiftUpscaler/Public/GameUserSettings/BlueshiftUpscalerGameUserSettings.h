// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// UE includes
#include "GameFramework/GameUserSettings.h"

#include "BlueshiftUpscalerGameUserSettings.generated.h"

/**
 *	Optional subclass of UGameUserSettings for use with the Blueshift Upscaler plugin.
 *	This interfaces with the Blueshift Upscaler Subsystem, but you can instead look at this to integrate into your own system.
 */
UCLASS(BlueprintType, meta = (DisplayName = "Upscaler Game User Settings (Blueshift)"))
class BLUESHIFTUPSCALER_API UBlueshiftUpscalerGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	// Begin UGameUserSettings interface
	virtual void SetToDefaults() override;
	virtual void SaveSettings() override;
	virtual void LoadSettings(bool bForceReload = false) override;
	virtual void ApplyNonResolutionSettings() override;
	// End UGameUserSettings interface
};