using UnrealBuildTool;
public class NepalOpenWorldEditorTarget : TargetRules
{
    public NepalOpenWorldEditorTarget(TargetInfo Info) : base(Info)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("NepalOpenWorld");
    }
}