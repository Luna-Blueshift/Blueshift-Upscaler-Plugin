// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Subsystem/BlueshiftUpscalerSubsystem.h"

/// Blueshift includes
#include "BlueshiftUpscalerLog.h"
#include "BlueshiftUpscalerUtils.h"

using namespace Blueshift::Upscaler;

void UBlueshiftUpscalerSubsystem::SaveSettings()
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftUpscalerSubsystem_SaveSettings);
	
	// Only instantiate/save Config for runtime objects
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// Log
	UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs"), __FUNCTION__);

	// Mirrors UGameUserSettings::SaveSettings in order to save to the same ini file
	// and ensure scalability persists
	// NOTE: We must also call this before the Super call to ensure our config is also saved with the cloud data
	Scalability::SaveState(GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni);
	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}

void UBlueshiftUpscalerSubsystem::LoadSettings(bool bForceReload)
{
	QUICK_SCOPE_CYCLE_COUNTER(BlueshiftUpscalerSubsystem_LoadSettings);
	
	// Only instantiate/load Config for runtime objects
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// Log
	UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs"), __FUNCTION__);

	// Load config from GameUserSettings.ini
	LoadConfig(nullptr, *GGameUserSettingsIni);
}

void UBlueshiftUpscalerSubsystem::ApplySettings()
{
	Utils::RunOnGameThread([this]()
	{
		UE_LOG(LogBlueshiftUpscaler, Log, TEXT("ApplySettings"));
		
		// Rendering options
		// Update which upscaler is enabled first, if any
		// We must do this first because they mutate r.ScreenPercentage, even when FSR3 is disabled, it will restore r.ScreenPercentage
		// so order is important here
		ResetUpscalerSettings();
		UpdateUpscalerEnabled();
	
		// Delay until next frame
		FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this](const float DeltaTime)
		{
			ApplyUpscalerSettings();

			// Do not tick again
			return false;
		}), 0.0f);
	});
}

void UBlueshiftUpscalerSubsystem::SetToDefaults()
{
	UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs"), __FUNCTION__);
	bDesiredFrameGen = false;
	SetDLSSToDefaults();
	SetFSRToDefaults();
	SetXeSSToDefaults();
}