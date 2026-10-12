using UnrealBuildTool;
public class NepalOpenWorld : ModuleRules
{
    public NepalOpenWorld(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "EnhancedInput", "UMG", "Slate", "SlateCore",
            "AIModule", "NavigationSystem"
        });
    }
}