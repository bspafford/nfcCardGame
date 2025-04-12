// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class cardGameClientTarget : TargetRules
{
    public cardGameClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		BuildEnvironment = TargetBuildEnvironment.Shared;
		ExtraModuleNames.Add("cardGame");
	}
}
