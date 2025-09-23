// Copyright (c) Blueshift Interactive Ltd (2025)

using EpicGames.Core;
using System.Linq;
using System.IO;
using UnrealBuildTool;

public static class BlueshiftIQUtils
{
	public static bool DoesPluginExistAndIsEnabled(ReadOnlyTargetRules Target, string PluginName)
	{
		// Project explicitly disables plugin?
		if (Target.DisablePlugins.Contains(PluginName))
		{
			return false;
		}
		
		// Look for the plugin .uplugin in project plugins recursively
		bool bFound = false;
		if (Target.ProjectFile != null)
		{
			var projectDir = Target.ProjectFile.Directory;
			bFound = Directory.EnumerateFiles(projectDir.FullName, PluginName + ".uplugin", SearchOption.AllDirectories).Any();
		}

		// If not found, look in engine plugins recursively
		if (bFound == false)
		{
			string EngineDir = Target.RelativeEnginePath;
			bFound = Directory.EnumerateFiles(EngineDir, PluginName + ".uplugin", SearchOption.AllDirectories).Any();
		}

		// Check project .uproject for explicit disable
		if (bFound && Target.ProjectFile != null)
		{
			var ProjectJson = JsonObject.Read(Target.ProjectFile);
			if (ProjectJson.TryGetObjectArrayField("Plugins", out var Plugins))
			{
				bool bExplicitlyDisabled = Plugins.Any(p =>
					p.TryGetStringField("Name", out var Name) && Name == PluginName
					&& p.TryGetBoolField("Enabled", out var bEnabled) && bEnabled == false
				);
				
				if (bExplicitlyDisabled)
				{
					return false;
				}
			}
		}

		return bFound;
	}
}

public class BlueshiftIQ : ModuleRules
{
	public BlueshiftIQ(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
		});
		
		PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"DeveloperSettings",
			"Engine",
			"RHI",
		});

		// Detect DLSS
		if (BlueshiftIQUtils.DoesPluginExistAndIsEnabled(Target, "DLSS"))
		{
			PublicDependencyModuleNames.Add("DLSSBlueprint");
			PublicDefinitions.Add("WITH_DLSS=1");
		}
		else
		{
			PublicDefinitions.Add("WITH_DLSS=0");
		}
		
		// Detect FSR
		if (BlueshiftIQUtils.DoesPluginExistAndIsEnabled(Target, "FSR4"))
		{
			PublicDependencyModuleNames.Add("FFXFSR4Settings");
			PublicDefinitions.Add("WITH_FSR4=1");
		}
		else if (BlueshiftIQUtils.DoesPluginExistAndIsEnabled(Target, "FSR3"))
		{
			PublicDependencyModuleNames.Add("FFXFSR3Settings");
			PublicDefinitions.Add("WITH_FSR3=1");
		}
		else
		{
			PublicDefinitions.Add("WITH_FSR4=0");
			PublicDefinitions.Add("WITH_FSR3=0");
		}
		
		// Detect XeSS
		if (BlueshiftIQUtils.DoesPluginExistAndIsEnabled(Target, "XeSS"))
		{
			PublicDependencyModuleNames.Add("XeSSBlueprint");
			PublicDependencyModuleNames.Add("XeFGBlueprint");
			PublicDefinitions.Add("WITH_XESS=1");
		}
		else
		{
			PublicDefinitions.Add("WITH_XESS=0");	
		}
	}
}