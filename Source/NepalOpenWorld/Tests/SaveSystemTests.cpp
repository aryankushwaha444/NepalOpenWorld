#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "SaveSystem/NOWSaveGame.h"
#include "Security/SaveValidation.h"
#include "Security/InputValidation.h"

IMPLEMENT_SIMPLE_AUTOMATIONTest_UNUSED_MACRO_PLACEHOLDER
// (macro line above must be deleted; kept only to show file boundary)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveCorruptClamped, "NepalOpenWorld.Save.CorruptValuesClamped", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) bool FSaveCorruptClamped::RunTest(const FString &)
{
    UNOWSaveGame *S = NewObject<UNOWSaveGame>();
    S->WalletNPR = -999999;
    S->Health = 5000.f;
    S->WantedLevel = 99;
    S->OwnedVehicleIds.Add(TEXT("V1"));
    S->OwnedVehicleIds.Add(TEXT("V1"));
    TestTrue("Validation repairs", USaveValidation::ValidateAndSanitize(S));
    TestTrue("Wallet clamped", S->WalletNPR == 0);
    TestTrue("Health clamped", S->Health == 100.f);
    TestTrue("Wanted clamped", S->WantedLevel == 5);
    TestTrue("Dup vehicles removed", S->OwnedVehicleIds.Num() == 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveSlotNameGuard, "NepalOpenWorld.Save.SlotNamePathTraversalBlocked", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FSaveSlotNameGuard::RunTest(const FString &)
{
    TestFalse("Traversal blocked", UInputValidation::IsValidSaveSlotName(TEXT("../../evil")));
    TestTrue("Normal name ok", UInputValidation::IsValidSaveSlotName(TEXT("NepalMain")));
    return true;
}