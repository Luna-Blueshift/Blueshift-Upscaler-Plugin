// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

namespace Blueshift::IQ::Utils
{
	void RunOnGameThread(const TFunction<void()>& Func);
	FVector2D GetScreenResolution();
}