// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class LearningProjectTarget : TargetRules
{
	public LearningProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "LearningProject" } );
	}
}
