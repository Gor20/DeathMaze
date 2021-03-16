// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Death_MAZETarget : TargetRules
{
	public Death_MAZETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Death_MAZE");
        DefaultBuildSettings = BuildSettingsVersion.V2;

    }
}
