// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class cardGame : ModuleRules
{
	public cardGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HTTP", "Json", "NetCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        PublicAdditionalLibraries.Add(Path.Combine(Target.WindowsPlatform.WindowsSdkDir,
                                        "Lib",
                                        Target.WindowsPlatform.WindowsSdkVersion,
                    "um/x64", // Probably a way to get this dynamically, but I didn't bother since it's a short run one off.
                    "winscard.lib"));
    }
}
