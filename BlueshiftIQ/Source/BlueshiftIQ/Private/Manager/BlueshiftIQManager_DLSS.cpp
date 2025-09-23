// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Manager/BlueshiftIQManager.h"

/// Blueshift includes
#include "BlueshiftIQPCH.h"
#include "BlueshiftIQLog.h"
#include "BlueshiftIQUtils.h"

/// DLSS includes
#if WITH_DLSS
#include "DLSSLibrary.h"
#endif

using namespace Blueshift::IQ;

bool UBlueshiftIQManager::IsDLSSSupported() const
{
#if WITH_DLSS
	return UDLSSLibrary::IsDLSSSupported();
#else
	return false;
#endif
}

void UBlueshiftIQManager::UpdateDLSSEnabled()
{
#if WITH_DLSS
	check(IsInGameThread());
	
	if (IsDLSSSupported())
	{
		UDLSSLibrary::EnableDLSS(DesiredUpscaler == EBlueshiftUpscalerMode::DLSS);
	}
#endif
}

void UBlueshiftIQManager::ApplyDLSSSettings()
{
#if WITH_DLSS
	check(IsInGameThread());
	
	if (DesiredUpscaler != EBlueshiftUpscalerMode::DLSS
		|| IsDLSSSupported() == false)
	{
		return;
	}
	
	// Resolve DLSS mode
	FixDLSSMode();
	const UDLSSMode DLSSPluginMode = static_cast<UDLSSMode>(DLSSMode);

	// Query DLSS info first
	// This is recommended by Nvidia's documentation
	const FVector2D ScreenResolution = Utils::GetScreenResolution();
	bool bIsSupported = false;
	float OptimalScreenPercentage;
	bool bIsFixedScreenPercentage;
	float MinScreenPercentage, MaxScreenPercentage, OptimalSharpness;
	UDLSSLibrary::GetDLSSModeInformation(DLSSPluginMode, ScreenResolution, OUT bIsSupported, OUT OptimalScreenPercentage, OUT bIsFixedScreenPercentage, OUT MinScreenPercentage, OUT MaxScreenPercentage, OUT OptimalSharpness);

	// Ignore this and use 100% if we're using DLAA
	// This is what Nvidia's documentation tells us to do
	if (DLSSPluginMode == UDLSSMode::DLAA)
	{
		OptimalScreenPercentage = 100.0f;
	}

	// Set the DLSS mode via mutating r.ScreenPercentage
	if (bIsSupported)
	{
		IConsoleVariable* const CVarScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
		const float OldScreenPercentage = CVarScreenPercentage->GetFloat();
		CVarScreenPercentage->Set(OptimalScreenPercentage);
		UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs: DLSS changed r.ScreenPercentage from %f to %f (desired: %f)"), __FUNCTION__, OldScreenPercentage, CVarScreenPercentage->GetFloat(), OptimalScreenPercentage);
	}
	// Warn
	else
	{
		UE_LOG(LogBlueshiftImageQuality, Warning, TEXT("%hs: DLSS Mode %s is not supported"), __FUNCTION__, *UEnum::GetDisplayValueAsText(DLSSPluginMode).ToString());
	}

	// Set DLSSRR
	UDLSSLibrary::EnableDLSSRR(bDLSSRREnabled);
#endif
}

void UBlueshiftIQManager::SetDLSSToDefaults()
{
#if WITH_DLSS
	DLSSMode = static_cast<int32>(UDLSSMode::Auto);
#else
	DLSSMode = 1; // 1 maps to Auto in UDLSSMode
#endif
	bDLSSRREnabled = false;
}

void UBlueshiftIQManager::FixDLSSMode()
{
#if WITH_DLSS
	// Clamp DLSS Mode to be within a valid range
	// 0 (Off) is not a valid range - if we're <=0, we'll get set to Auto
	const int32 MinIndex = static_cast<int32>(UDLSSMode::Auto);
	const int32 MaxIndex = static_cast<int32>(UDLSSMode::UltraPerformance);
	DLSSMode = FMath::Clamp(DLSSMode, MinIndex, MaxIndex);
#endif
}

void UBlueshiftIQManager::SetDLSSMode(int32 InDLSSMode)
{
	DLSSMode = InDLSSMode;
	FixDLSSMode();
}

void UBlueshiftIQManager::SetDLSSRREnabled(const bool bInEnabled)
{
#if WITH_DLSS
	if (bInEnabled
		&& UDLSSLibrary::IsDLSSRRSupported())
	{
		bDLSSRREnabled = true;
	}
	else
	{
		bDLSSRREnabled = false;
	}
#else
	bDLSSRREnabled = false;
#endif
}

bool UBlueshiftIQManager::IsDLSSRRSupported() const
{
#if WITH_DLSS
	return UDLSSLibrary::IsDLSSRRSupported();
#else
	return false;
#endif
}