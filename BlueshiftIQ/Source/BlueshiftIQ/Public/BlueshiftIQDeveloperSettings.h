// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// UE includes
#include "Engine/DeveloperSettings.h"

#include "BlueshiftIQDeveloperSettings.generated.h"

UCLASS(Config = "Engine", DefaultConfig, meta = (DisplayName = "Image Quality Settings"))
class BLUESHIFTIQ_API UBlueshiftIQDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UBlueshiftIQDeveloperSettings* Get() { return GetDefault<UBlueshiftIQDeveloperSettings>(); }
	
	// Begin UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return TEXT("Blueshift"); }
	// End UDeveloperSettings interface

	/** Handle Anti-Aliasing? */
	UPROPERTY(EditAnywhere, Config, Category = "Image Quality")
	bool bHandleAA = false;
};