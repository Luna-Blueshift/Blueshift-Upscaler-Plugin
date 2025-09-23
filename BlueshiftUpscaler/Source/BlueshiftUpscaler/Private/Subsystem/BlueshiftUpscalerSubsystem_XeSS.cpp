// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Subsystem/BlueshiftUpscalerSubsystem.h"

/// Blueshift includes
#include "BlueshiftUpscalerPCH.h"
#include "BlueshiftUpscalerLog.h"
#include "BlueshiftUpscalerUtils.h"

/// XeSS includes
#if WITH_XESS
#include "XeFGBlueprintLibrary.h"
#include "XeSSBlueprintLibrary.h"
#endif

using namespace Blueshift::Upscaler;

bool UBlueshiftUpscalerSubsystem::IsXeSSSupported() const
{
#if WITH_XESS
	return UXeSSBlueprintLibrary::IsXeSSSupported();
#else
	return false;
#endif
}

bool UBlueshiftUpscalerSubsystem::IsXeFGSupported() const
{
#if WITH_XESS
	return UXeFGBlueprintLibrary::IsXeFGSupported();
#else
	return false;
#endif
}

void UBlueshiftUpscalerSubsystem::UpdateXeSSEnabled()
{
#if WITH_XESS
	check(IsInGameThread());
	
	if (IsXeSSSupported())
	{
		if (IConsoleVariable* const CVarXeSSEnabled = IConsoleManager::Get().FindConsoleVariable(TEXT("r.XeSS.Enabled")))
		{
			CVarXeSSEnabled->Set(DesiredUpscaler == EBlueshiftUpscalerMode::XeSS);
		}
	}
#endif
}

void UBlueshiftUpscalerSubsystem::ApplyXeSSSettings()
{
#if WITH_XESS
	check(IsInGameThread());
	
	if (DesiredUpscaler != EBlueshiftUpscalerMode::XeSS)
	{
		return;
	}

	// XeSS
	if (IsXeSSSupported())
	{
		IConsoleVariable* const CVarScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
		const float OldScreenPercentage = CVarScreenPercentage->GetFloat();

		// Resolve Quality Mode
		FixXeSSQualityMode();
		const EXeSSQualityMode PluginQualityMode = static_cast<EXeSSQualityMode>(XeSSQualityMode);
	
		// Turn off by default because we'll turn this back on when we call ApplyXeSSSettings
		UXeSSBlueprintLibrary::SetXeSSQualityMode(PluginQualityMode);

		// Log r.ScreenPercentage change
		const float NewScreenPercentage = CVarScreenPercentage->GetFloat();
		UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs: XeSS changed r.ScreenPercentage from %f to %f"), __FUNCTION__, OldScreenPercentage, NewScreenPercentage);
	}

	// XeFG
	if (IsXeFGSupported())
	{
		// Set Frame Gen mode
		UXeFGBlueprintLibrary::SetXeFGMode(bDesiredFrameGen ? EXeFGMode::On : EXeFGMode::Off);
		UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs: XeFG: %s"), __FUNCTION__, bDesiredFrameGen ? TEXT("on") : TEXT("off"));
	}
#endif
}

void UBlueshiftUpscalerSubsystem::SetXeSSToDefaults()
{
	XeSSQualityMode = 1;	
}

void UBlueshiftUpscalerSubsystem::FixXeSSQualityMode()
{
#if WITH_XESS
	const int32 MinValue = static_cast<int32>(EXeSSQualityMode::UltraPerformance);
	const int32 MaxValue = static_cast<int32>(EXeSSQualityMode::AntiAliasing);
	XeSSQualityMode = FMath::Clamp(XeSSQualityMode, MinValue, MaxValue);
#endif
}

void UBlueshiftUpscalerSubsystem::SetXeSSQualityMode(const int32 InQualityMode)
{
	XeSSQualityMode = InQualityMode;
	FixXeSSQualityMode();
}