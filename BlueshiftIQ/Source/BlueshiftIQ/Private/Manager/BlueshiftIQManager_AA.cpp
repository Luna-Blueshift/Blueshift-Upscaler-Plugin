// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Manager/BlueshiftIQManager.h"

/// UE includes
#include "Engine/RendererSettings.h"
#include "RHI.h"

/// Blueshift includes
#include "BlueshiftIQPCH.h"
#include "BlueshiftIQDeveloperSettings.h"
#include "BlueshiftIQLog.h"
#include "BlueshiftIQUtils.h"
#include "Types/BlueshiftAAMethod.h"

using namespace Blueshift::IQ;

void UBlueshiftIQManager::ApplyAASettings()
{
	// Exit out if we're not handling AA here
	if (UBlueshiftIQDeveloperSettings::Get()->bHandleAA == false)
	{
		return;
	}

	if (IConsoleVariable* const CVarAAMethod = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod")))
	{
		const int32 iAAMethod = static_cast<int32>(AAMethod);
		CVarAAMethod->Set(iAAMethod);
	}
}

void UBlueshiftIQManager::SetAAToDefaults()
{
	AAMethod = EBlueshiftAAMethod::TAA;
}

void UBlueshiftIQManager::SetAAMethod(const EBlueshiftAAMethod InAAMethod)
{
	if (IsAAMethodSupported(InAAMethod))
	{
		AAMethod = InAAMethod;
	}
	// Warn
	else
	{
		UE_LOG(LogBlueshiftImageQuality, Warning, TEXT("%hs: Tried to set AA Method to %s, but it is not supported on this platform"),
			__FUNCTION__,
			*UEnum::GetDisplayValueAsText(InAAMethod).ToString()
		);
	}
}

bool IsMobile()
{
	FStaticShaderPlatform Platform = GShaderPlatformForFeatureLevel[GMaxRHIFeatureLevel];
	return IsMobilePlatform(GMaxRHIShaderPlatform);
}

bool IsForward()
{
	return GetDefault<URendererSettings>()->bForwardShading;
}

bool UBlueshiftIQManager::IsAAMethodSupported(const EBlueshiftAAMethod InAAMethod) const
{
	switch (InAAMethod)
	{
		case EBlueshiftAAMethod::None:
		{
			// Always available
			return true;
		}

		case EBlueshiftAAMethod::FXAA:
		{
			// Always available
			return true;
		}

		case EBlueshiftAAMethod::TAA:
		{
			// Not available on mobile
			return IsMobile() == false;
		}

		case EBlueshiftAAMethod::MSAA:
		{
			// Only available on mobile or forward
			return IsMobile() || IsForward();
		}

		case EBlueshiftAAMethod::TSR:
		{
			// Only available in UE5 on the desktop renderer
#if ENGINE_MAJOR_VERSION >= 5
			return IsMobile() == false;
#else
			return false;
#endif
			break;
		}
		
		case EBlueshiftAAMethod::SMAA:
		{
			// 5.7
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 7
			return true;
#else
			return false;
#endif
			break;
		}
		
		default:
		{
			UE_LOG(LogBlueshiftImageQuality, Warning, TEXT("%hs: AA Method %s is undefined"), __FUNCTION__, *UEnum::GetDisplayValueAsText(InAAMethod).ToString());
			break;
		}
	}

	return false;
}

TArray<EBlueshiftAAMethod> UBlueshiftIQManager::GetSupportedAAMethods() const
{
	TArray<EBlueshiftAAMethod> Result;
	for (const EBlueshiftAAMethod AA : TEnumRange<EBlueshiftAAMethod>())
	{
		if (IsAAMethodSupported(AA))
		{
			Result.Emplace(AA);
		}
	}
	return Result;
}