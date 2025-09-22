// Copyright (c) Blueshift Interactive Ltd (2025)

#include "BlueshiftUpscalerUtils.h"

/// UE includes
#include "GameFramework/GameUserSettings.h"

using namespace Blueshift::Upscaler;

void Utils::RunOnGameThread(const TFunction<void()>& Func)
{
	if (Func == nullptr)
	{
		return;
	}

	if (IsInGameThread())
	{
		Func();
	}
	else
	{
		AsyncTask(ENamedThreads::GameThread, [F = Func]()
		{
			if (F != nullptr)
			{
				F();
			}
		});
	}
}

FVector2D Utils::GetScreenResolution()
{
	const UGameUserSettings* const GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (GameUserSettings == nullptr)
	{
		return FVector2D::ZeroVector;
	}

	const FIntPoint ScreenResolution = GameUserSettings->GetScreenResolution();
	return FVector2D(static_cast<float>(ScreenResolution.X), static_cast<float>(ScreenResolution.Y));
}