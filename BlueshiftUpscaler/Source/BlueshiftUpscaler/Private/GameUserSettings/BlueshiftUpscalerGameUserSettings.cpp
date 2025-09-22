// Copyright (c) Blueshift Interactive Ltd (2025)

#include "GameUserSettings/BlueshiftUpscalerGameUserSettings.h"

/// Blueshift includes
#include "Subsystem/BlueshiftUpscalerSubsystem.h"

// Begin UGameUserSettings interface
void UBlueshiftUpscalerGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftUpscalerSubsystem* const UpscalerSubsystem = UBlueshiftUpscalerSubsystem::Get())
		{
			UpscalerSubsystem->SetToDefaults();
		}
	}
}

void UBlueshiftUpscalerGameUserSettings::SaveSettings()
{
	Super::SaveSettings();

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftUpscalerSubsystem* const UpscalerSubsystem = UBlueshiftUpscalerSubsystem::Get())
		{
			UpscalerSubsystem->SaveSettings();
		}
	}
}

void UBlueshiftUpscalerGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftUpscalerSubsystem* const UpscalerSubsystem = UBlueshiftUpscalerSubsystem::Get())
		{
			UpscalerSubsystem->LoadSettings();
		}
	}
}

void UBlueshiftUpscalerGameUserSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftUpscalerSubsystem* const UpscalerSubsystem = UBlueshiftUpscalerSubsystem::Get())
		{
			UpscalerSubsystem->ApplySettings();
		}
	}
}
// End UGameUserSettings interface