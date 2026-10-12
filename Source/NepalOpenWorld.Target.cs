using UnrealBuildTool;
public class NepalOpenWorldTarget : TargetRules
{
    public NepalOpenWorldTarget(TargetInfo Info) : base(Info)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("NepalOpenWorld");
    }
}