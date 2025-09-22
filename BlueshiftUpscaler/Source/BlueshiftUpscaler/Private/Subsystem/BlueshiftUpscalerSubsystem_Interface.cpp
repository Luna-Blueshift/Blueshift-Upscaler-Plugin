// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Subsystem/BlueshiftUpscalerSubsystem.h"

/// Blueshift includes
#include "BlueshiftUpscalerLog.h"

void UBlueshiftUpscalerSubsystem::ResetUpscalerSettings()
{
	check(IsInGameThread());
	IConsoleVariable* const CVarScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (ensureAlways(CVarScreenPercentage != nullptr))
	{
		CVarScreenPercentage->Set(100.0f);
	}
}

void UBlueshiftUpscalerSubsystem::UpdateUpscalerEnabled()
{
	check(IsInGameThread());
	UpdateDLSSEnabled();
	UpdateFSREnabled();
	UpdateXeSSEnabled();
}

void UBlueshiftUpscalerSubsystem::ApplyUpscalerSettings()
{
	check(IsInGameThread());
	
	switch (DesiredUpscaler)
	{
		case EBlueshiftUpscalerMode::None:
		{
			break;
		}

		case EBlueshiftUpscalerMode::DLSS:
		{
			ApplyDLSSSettings();
			break;
		}
	
		case EBlueshiftUpscalerMode::FSR:
		{
			ApplyFSRSettings();
			break;
		}
		
		case EBlueshiftUpscalerMode::XeSS:
		{
			ApplyXeSSSettings();
			break;
		}

		// Undefined - error
		default:
		{
			UE_LOG(LogBlueshiftUpscaler, Warning, TEXT("%hs: Upscaler is undefined"), __FUNCTION__);
			break;
		}
	}
}

bool UBlueshiftUpscalerSubsystem::SetDesiredUpscaler(const EBlueshiftUpscalerMode InUpscalerMode)
{
	// Validate that the upscaler is actually supported first
	if (IsUpscalerSupported(InUpscalerMode) == false)
	{
		UE_LOG(LogBlueshiftUpscaler, Warning, TEXT("%hs: Upscaler %s is not supported"),
			__FUNCTION__,
			*UEnum::GetDisplayValueAsText(InUpscalerMode).ToString()
		);
		return false;
	}
	
	if (InUpscalerMode == DesiredUpscaler)
	{
		return true;
	}

	// Mutate the desired upscaler
	const EBlueshiftUpscalerMode OldDesiredUpscaler = DesiredUpscaler;
	DesiredUpscaler = InUpscalerMode;

	// Log
	UE_LOG(LogBlueshiftUpscaler, Log, TEXT("%hs: Desired upscaler changed from %s to %s"),
		__FUNCTION__,
		*UEnum::GetDisplayValueAsText(OldDesiredUpscaler).ToString(),
		*UEnum::GetDisplayValueAsText(InUpscalerMode).ToString()
	);

	return true;
}

bool UBlueshiftUpscalerSubsystem::IsUpscalerSupported(const EBlueshiftUpscalerMode InUpscalerMode) const
{
	switch (InUpscalerMode)
	{
		case EBlueshiftUpscalerMode::None:
		{
			return true;
		}

		case EBlueshiftUpscalerMode::DLSS:
		{
			return IsDLSSSupported();
		}
	
		case EBlueshiftUpscalerMode::FSR:
		{
			return IsFSRSupported();
		}
		
		case EBlueshiftUpscalerMode::XeSS:
		{
			return IsXeSSSupported();
		}

		// Undefined - error
		default:
		{
			UE_LOG(LogBlueshiftUpscaler, Warning, TEXT("%hs: Upscaler is undefined"), __FUNCTION__);
			break;
		}
	}

	return false;
}

TArray<EBlueshiftUpscalerMode> UBlueshiftUpscalerSubsystem::GetSupportedUpscalers() const
{
	TArray<EBlueshiftUpscalerMode> Result;

	if (IsDLSSSupported())
	{
		Result.Emplace(EBlueshiftUpscalerMode::DLSS);
	}
	if (IsFSRSupported())
	{
		Result.Emplace(EBlueshiftUpscalerMode::FSR);
	}
	if (IsXeSSSupported())
	{
		Result.Emplace(EBlueshiftUpscalerMode::XeSS);
	}

	return Result;
}