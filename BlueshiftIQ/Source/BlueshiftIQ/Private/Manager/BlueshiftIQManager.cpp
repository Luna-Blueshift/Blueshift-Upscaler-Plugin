// Copyright (c) Blueshift Interactive Ltd (2025)

#include "Manager/BlueshiftIQManager.h"

UBlueshiftIQManager* UBlueshiftIQManager::Get()
{
	return GEngine != nullptr ? GEngine->GetEngineSubsystem<UBlueshiftIQManager>() : nullptr;
}