// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class trainingProj : ModuleRules
{
	public trainingProj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"trainingProj",
			"trainingProj/Variant_Platforming",
			"trainingProj/Variant_Platforming/Animation",
			"trainingProj/Variant_Combat",
			"trainingProj/Variant_Combat/AI",
			"trainingProj/Variant_Combat/Animation",
			"trainingProj/Variant_Combat/Gameplay",
			"trainingProj/Variant_Combat/Interfaces",
			"trainingProj/Variant_Combat/UI",
			"trainingProj/Variant_SideScrolling",
			"trainingProj/Variant_SideScrolling/AI",
			"trainingProj/Variant_SideScrolling/Gameplay",
			"trainingProj/Variant_SideScrolling/Interfaces",
			"trainingProj/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
