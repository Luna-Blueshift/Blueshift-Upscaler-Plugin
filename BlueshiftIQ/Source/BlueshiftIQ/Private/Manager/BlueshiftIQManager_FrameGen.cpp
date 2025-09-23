// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Manager/BlueshiftIQManager.h"

/// Blueshift includes
#include "BlueshiftIQLog.h"

void UBlueshiftIQManager::SetDesiredFrameGen(const bool bEnabled)
{
	bDesiredFrameGen = bEnabled;
}

bool UBlueshiftIQManager::IsFrameGenSupported(const EBlueshiftUpscalerMode InUpscalerMode) const
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
			UE_LOG(LogBlueshiftImageQuality, Warning, TEXT("%hs: Upscaler is undefined"), __FUNCTION__);
			break;
		}
	}

	return false;
}
