// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// UE includes
#include "Subsystems/EngineSubsystem.h"

/// Blueshift includes
#include "BlueshiftUpscalerMode.h"

#include "BlueshiftUpscalerSubsystem.generated.h"

UCLASS(Config = "GameUserSettings", ConfigDoNotCheckDefaults, BlueprintType, meta = (DisplayName = "Upscaler Subsystem (Blueshift)"))
class BLUESHIFTUPSCALER_API UBlueshiftUpscalerSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UBlueshiftUpscalerSubsystem* Get();

	//-----------------------------------------------------------------------------------------------------------------
	//	SETTINGS
	//-----------------------------------------------------------------------------------------------------------------
public:
	/** Save to GameUserSettings.ini */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveSettings();
	
	/** Loads from GameUserSettings.ini */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadSettings(bool bForceReload = false);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplySettings();
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetToDefaults();

	//-----------------------------------------------------------------------------------------------------------------
	//	INTERFACE
	//-----------------------------------------------------------------------------------------------------------------
private:
	/** Upscaler we would like to use. */
	UPROPERTY(VisibleAnywhere, Config, Category = "Upscaler")
	EBlueshiftUpscalerMode DesiredUpscaler = EBlueshiftUpscalerMode::None;

	void ResetUpscalerSettings();
	void UpdateUpscalerEnabled();
	void ApplyUpscalerSettings();

public:
	UFUNCTION(BlueprintPure, Category = "Upscaler")
	FORCEINLINE EBlueshiftUpscalerMode GetDesiredUpscaler() const { return DesiredUpscaler; }

	/**
	 * Set the upscaler we want to use. This does not guarantee we will use it however - the plugin must exist, be enabled, and the hardware must support it.
	 * If successful, you will need to call ApplySettings to actually setup the upscaler.
	 * @return Whether upscaler was successfully set
	 */
	UFUNCTION(BlueprintCallable, Category = "Upscaler")
	bool SetDesiredUpscaler(const EBlueshiftUpscalerMode InUpscalerMode);

	/** Is the given upscaler currently supported? The plugin must exist, be enabled, and the hardware must support it. */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Upscaler")
	bool IsUpscalerSupported(const EBlueshiftUpscalerMode InUpscalerMode) const;

	/** Return a list of all the upscalers which are currently supported. */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Upscaler")
	TArray<EBlueshiftUpscalerMode> GetSupportedUpscalers() const;

	//-----------------------------------------------------------------------------------------------------------------
	//	DLSS
	//-----------------------------------------------------------------------------------------------------------------
private:
	/** Is DLSS DLSR (Super Resolution) or DLAA supported? */
	bool IsDLSSSupported() const;
	
	void UpdateDLSSEnabled();
	void ApplyDLSSSettings();
	void SetDLSSToDefaults();

	/**
	 *	Mode which maps to UDLSSMode. This controls whether DLSS is enabled, and which mode to use between DLAA, and varying Super Resolution quality modes.
	 *	0 = Off <- Should never be set 0 because this is controlled by our DesiredUpscaler.
	 *	1 = Auto-Detect
	 *	2 = DLAA
	 *	3 = Ultra Quality
	 *	4 = Quality
	 *	5 = Balanced
	 *	6 = Performance
	 *	7 = Ultra Performance
	 */
	UPROPERTY(Config)
	int32 DLSSMode = 1;

	/** Is Ray Reconstruction enabled? */
	UPROPERTY(Config)
	bool bDLSSRREnabled = false;

	/** Fixes the DLSS Mode so it points to a valid mode. */
	void FixDLSSMode();

public:
	UFUNCTION(BlueprintPure, Category = "DLSS")
	FORCEINLINE int32 GetDLSSMode() const { return DLSSMode; }
	
	UFUNCTION(BlueprintCallable, Category = "DLSS")
	void SetDLSSMode(int32 InDLSSMode);
	
	UFUNCTION(BlueprintPure, Category = "DLSS")
	FORCEINLINE bool GetDLSSRREnabled() const { return bDLSSRREnabled; }

	UFUNCTION(BlueprintCallable, Category = "DLSS")
	void SetDLSSRREnabled(const bool bInEnabled);

	UFUNCTION(BlueprintPure, Category = "DLSS")
	bool IsDLSSRRSupported() const;

	//-----------------------------------------------------------------------------------------------------------------
	//	FSR
	//-----------------------------------------------------------------------------------------------------------------
private:
	bool IsFSRSupported() const;
	bool IsFFISupported() const;
	void UpdateFSREnabled();
	void ApplyFSRSettings();
	void SetFSRToDefaults();

	/**
	 *	Mode which maps to r.FidelityFX.FSR4.QualityMode.
	 *	0 = Native AA
	 *	1 = Quality
	 *	2 = Balanced
	 *	3 = Performance
	 *	4 = Ultra Performance
	 */
	UPROPERTY(Config)
	int32 FSRMode = 0;

	/**
	 *	Sharpness for FSR.
	 *	Range [0, 20]
	 */
	UPROPERTY(Config)
	float FSRSharpness = 0.0f;

	void FixFSRMode();
	void FixFSRSharpness();

public:
	UFUNCTION(BlueprintPure, Category = "FSR")
	FORCEINLINE int32 GetFSRMode() const { return FSRMode; }
	
	UFUNCTION(BlueprintCallable, Category = "FSR")
	void SetFSRMode(const int32 InFSRMode);

	UFUNCTION(BlueprintPure, Category = "FSR")
	FORCEINLINE float GetFSRSharpness() const { return FSRSharpness; }
	
	UFUNCTION(BlueprintCallable, Category = "FSR")
	void SetFSRSharpness(const float InSharpness);

	//-----------------------------------------------------------------------------------------------------------------
	//	XeSS
	//-----------------------------------------------------------------------------------------------------------------
private:
	bool IsXeSSSupported() const;
	bool IsXeFGSupported() const;
	void UpdateXeSSEnabled();
	void ApplyXeSSSettings();
	void SetXeSSToDefaults();

	/**
	 *	Mode which maps to EXeSSQualityMode.
	 *	0 = Off <- this should never be set to 0
	 *	1 = Ultra Performance
	 *	2 = Performance
	 *	3 = Balanced
	 *	4 = Quality
	 *	5 = Ultra Quality
	 *	6 = Ultra Quality Plus
	 *	7 = Anti-Aliasing
	 */
	UPROPERTY(Config)
	int32 XeSSQualityMode = 1;
	
	void FixXeSSQualityMode();

public:
	UFUNCTION(BlueprintPure, Category = "XeSS", meta = (DisplayName = "Get XeSS Quality Mode"))
	FORCEINLINE int32 GetXeSSQualityMode() const { return XeSSQualityMode; }

	UFUNCTION(BlueprintCallable, Category = "XeSS", meta = (DisplayName = "Set XeSS Quality Mode"))
	void SetXeSSQualityMode(const int32 InQualityMode);

	//-----------------------------------------------------------------------------------------------------------------
	//	FRAME GEN
	//-----------------------------------------------------------------------------------------------------------------
private:
	/**
	 *	Do we want to turn on frame gen if possible?
	 *	DLSS = Automatic, so this is not necessary
	 *	FSR = FFI (frame interpolation)
	 *	XeSS = XeSS-FG (frame gen)
	 */
	UPROPERTY(Config)
	bool bDesiredFrameGen = false;

public:
	UFUNCTION(BlueprintPure, Category = "FrameGen")
	FORCEINLINE bool GetDesiredFrameGen() const { return bDesiredFrameGen; }
	
	UFUNCTION(BlueprintCallable, Category = "FrameGen")
	void SetDesiredFrameGen(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "FrameGen")
	bool IsFrameGenSupported(const EBlueshiftUpscalerMode InUpscalerMode) const;
};