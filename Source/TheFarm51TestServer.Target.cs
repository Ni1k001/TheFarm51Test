// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class TheFarm51TestServerTarget : TargetRules   // Change this line as shown previously
{
       public TheFarm51TestServerTarget(TargetInfo Target) : base(Target)  // Change this line as shown previously
       {
        Type = TargetType.Server;
        ExtraModuleNames.Add("TheFarm51Test");    // Change this line as shown previously
       }
}