// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Manager/BlueshiftIQManager.h"

/// Blueshift includes
#include "BlueshiftIQLog.h"
#include "BlueshiftIQUtils.h"

using namespace Blueshift::IQ;

void UBlueshiftIQManager::SaveSettings()
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftIQManager_SaveSettings);
	
	// Only instantiate/save Config for runtime objects
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// Log
	UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs"), __FUNCTION__);

	// Mirrors UGameUserSettings::SaveSettings in order to save to the same ini file
	// and ensure scalability persists
	// NOTE: We must also call this before the Super call to ensure our config is also saved with the cloud data
	Scalability::SaveState(GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni);
	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}

void UBlueshiftIQManager::LoadSettings(bool bForceReload)
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftIQManager_LoadSettings);
	
	// Only instantiate/load Config for runtime objects
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// Log
	UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs"), __FUNCTION__);

	// Load config from GameUserSettings.ini
	LoadConfig(nullptr, *GGameUserSettingsIni);
}

void UBlueshiftIQManager::ApplySettings()
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftIQManager_ApplySettings);
	
	Utils::RunOnGameThread([this]()
	{
		UE_LOG(LogBlueshiftImageQuality, Log, TEXT("ApplySettings"));
		
		// Rendering options
		// Update which upscaler is enabled first
		// We must do this first because they all mutate r.ScreenPercentage, and TSR may even depend on it for render scale
		// so order is important here
		ResetUpscalerSettings();
		UpdateUpscalerEnabled();
	
		// Delay until next frame
		FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this](const float DeltaTime)
		{
			ApplyUpscalerSettings();
			ApplyAASettings();

			// Do not tick again
			return false;
		}), 0.0f);
	});
}

void UBlueshiftIQManager::SetToDefaults()
{
	UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs"), __FUNCTION__);
	bDesiredFrameGen = false;
	SetAAToDefaults();
	SetDLSSToDefaults();
	SetFSRToDefaults();
	SetXeSSToDefaults();
}