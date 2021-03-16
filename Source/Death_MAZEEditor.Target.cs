// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Death_MAZEEditorTarget : TargetRules
{
	public Death_MAZEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Death_MAZE");
		DefaultBuildSettings = BuildSettingsVersion.V2;
		
	}
}
