// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Subsystem/BlueshiftUpscalerSubsystem.h"

/// Blueshift includes
#include "BlueshiftUpscalerLog.h"

void UBlueshiftUpscalerSubsystem::SetDesiredFrameGen(const bool bEnabled)
{
	bDesiredFrameGen = bEnabled;
}

bool UBlueshiftUpscalerSubsystem::IsFrameGenSupported(const EBlueshiftUpscalerMode InUpscalerMode) const
{
	switch (InUpscalerMode)
	{
		case EBlueshiftUpscalerMode::None:
		{
			break;
		}

		case EBlueshiftUpscalerMode::DLSS:
		{
			// Frame Generation in DLSS is tied to the version and can't be manually toggled as far as I know
			// If this isn't the case, we'll revisit this part
			return false;
		}
	
		case EBlueshiftUpscalerMode::FSR:
		{
			return IsFFISupported();
		}
		
		case EBlueshiftUpscalerMode::XeSS:
		{
			return IsXeFGSupported();
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
