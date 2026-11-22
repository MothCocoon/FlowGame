// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors

using UnrealBuildTool;

public class FlowGameEditorTarget : TargetRules
{
	public FlowGameEditorTarget(TargetInfo target) : base(target)
	{
		Type = TargetType.Editor;

		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
		
		ExtraModuleNames.AddRange(
		[
			"FlowGame"
		]);
    }
}
