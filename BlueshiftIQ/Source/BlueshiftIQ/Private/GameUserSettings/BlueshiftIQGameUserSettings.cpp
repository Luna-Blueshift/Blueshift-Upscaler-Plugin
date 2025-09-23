// Copyright (c) Blueshift Interactive Ltd (2025)

#include "GameUserSettings/BlueshiftIQGameUserSettings.h"

/// Blueshift includes
#include "Manager/BlueshiftIQManager.h"

// Begin UGameUserSettings interface
void UBlueshiftIQGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftIQManager* const Manager = UBlueshiftIQManager::Get())
		{
			Manager->SetToDefaults();
		}
	}
}

void UBlueshiftIQGameUserSettings::SaveSettings()
{
	Super::SaveSettings();

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftIQManager* const Manager = UBlueshiftIQManager::Get())
		{
			Manager->SaveSettings();
		}
	}
}

void UBlueshiftIQGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftIQManager* const Manager = UBlueshiftIQManager::Get())
		{
			Manager->LoadSettings();
		}
	}
}

void UBlueshiftIQGameUserSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		if (UBlueshiftIQManager* const Manager = UBlueshiftIQManager::Get())
		{
			Manager->ApplySettings();
		}
	}
}
// End UGameUserSettings interface