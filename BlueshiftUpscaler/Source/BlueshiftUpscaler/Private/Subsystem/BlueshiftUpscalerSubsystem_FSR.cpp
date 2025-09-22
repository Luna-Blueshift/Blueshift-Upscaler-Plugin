// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Subsystem/BlueshiftUpscalerSubsystem.h"

/// Blueshift includes
#include "BlueshiftUpscalerPCH.h"
#include "BlueshiftUpscalerLog.h"
#include "BlueshiftUpscalerUtils.h"

/// FSR includes
#if WITH_FSR4
#include "FFXFSR4Settings.h"
#elif WITH_FSR3
#include "FFXFSR3Settings.h"
#endif

using namespace Blueshift::Upscaler;

bool UBlueshiftUpscalerSubsystem::IsFSRSupported() const
{
	return WITH_FSR;
}

void UBlueshiftUpscalerSubsystem::UpdateFSREnabled()
{
#if WITH_FSR
	check(IsInGameThread());
	
	if (IsFSRSupported())
	{
#if WITH_FSR4
		CVarEnableFSR4->Set(DesiredUpscaler == EBlueshiftUpscalerMode::FSR);
#elif WITH_FSR3
		CVarEnableFSR3->Set(UpscalerType == EBlueshiftUpscalerType::FSR);
#endif
	}
#endif
}

void UBlueshiftUpscalerSubsystem::ApplyFSRSettings()
{
#if WITH_FSR
	check(IsInGameThread());
	
	if (DesiredUpscaler != EBlueshiftUpscalerMode::FSR
		|| IsFSRSupported() == false)
	{
		return;
	}
	
	IConsoleVariable* const CVarScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	const float OldScreenPercentage = CVarScreenPercentage->GetFloat();
	
	// Quality Mode
	FixFSRMode();
	const int32 OldQualityMode = CVarFSR4QualityMode->GetInt();
#if WITH_FSR4
	CVarFSR4QualityMode->Set(FSRMode);
#elif WITH_FSR3
	CVarFSR3QualityMode->Set(FSRMode);
#endif
	UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs: FSR Quality Mode changed from %i to %i (desired: %i)"), __FUNCTION__, OldQualityMode, CVarFSR4QualityMode->GetInt(), FSRMode);
	UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs: FSR changed r.ScreenPercentage from %f to %f"), __FUNCTION__, OldScreenPercentage, CVarScreenPercentage->GetFloat());

	// Sharpness
	FixFSRSharpness();
	const float OldSharpness = CVarFSR4Sharpness->GetFloat();
#if WITH_FSR4
	CVarFSR4Sharpness->Set(FSRSharpness);
#elif WITH_FSR3
	CVarFSR3Sharpness->Set(FSRSharpness);
#endif
	UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs: FSR Sharpness changed from %f to %f (desired: %f)"), __FUNCTION__, OldSharpness, CVarFSR4Sharpness->GetFloat(), FSRSharpness);
#endif
}

void UBlueshiftUpscalerSubsystem::SetFSRToDefaults()
{
	FSRMode = 0;
	FSRSharpness = 0.0f;
}

void UBlueshiftUpscalerSubsystem::FixFSRMode()
{
	// Quality Modes defined in CVarFSR4QualityMode
	FSRMode = FMath::Clamp(FSRMode, 0, 4);
}

void UBlueshiftUpscalerSubsystem::FixFSRSharpness()
{
	FSRSharpness = FMath::Max(FSRSharpness, 0.0f);
}

void UBlueshiftUpscalerSubsystem::SetFSRMode(const int32 InFSRMode)
{
	FSRMode = InFSRMode;
	FixFSRMode();
}

void UBlueshiftUpscalerSubsystem::SetFSRSharpness(const float InSharpness)
{
	FSRSharpness = InSharpness;
	FixFSRSharpness();
}