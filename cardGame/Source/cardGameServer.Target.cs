// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class cardGameServerTarget : TargetRules
{
    public cardGameServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        BuildEnvironment = TargetBuildEnvironment.Shared;
        ExtraModuleNames.Add("cardGame");
    }
}
