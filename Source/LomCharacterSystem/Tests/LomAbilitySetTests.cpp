#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "AbilitySystem/LomAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Engine/World.h"
#include "Equipment/LomEquipmentItemInstance.h"
#include "GameFramework/Pawn.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLomEquipmentAbilitySetIsolationTest,
	"Lom.Equipment.AbilitySetIsolation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FLomEquipmentAbilitySetIsolationTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	if (!TestNotNull(TEXT("Test world created"), World))
	{
		return false;
	}
	APawn* FirstPlayer = World->SpawnActor<APawn>();
	APawn* SecondPlayer = World->SpawnActor<APawn>();
	UAbilitySystemComponent* FirstASC = NewObject<UAbilitySystemComponent>(FirstPlayer);
	UAbilitySystemComponent* SecondASC = NewObject<UAbilitySystemComponent>(SecondPlayer);
	FirstPlayer->AddInstanceComponent(FirstASC);
	SecondPlayer->AddInstanceComponent(SecondASC);
	FirstASC->RegisterComponent();
	SecondASC->RegisterComponent();
	FirstASC->InitAbilityActorInfo(FirstPlayer, FirstPlayer);
	SecondASC->InitAbilityActorInfo(SecondPlayer, SecondPlayer);

	ULomAbilitySet* SharedSet = NewObject<ULomAbilitySet>();
	FGrantedAbilityItem& Ability = SharedSet->Abilities.AddDefaulted_GetRef();
	Ability.AbilityClass = ULomGameplayAbility::StaticClass();
	Ability.InputTag = FGameplayTag::RequestGameplayTag(TEXT("UI.Anchor.Crosshair"));
	SharedSet->Attributes.AddDefaulted_GetRef().AttributeSet = UAttributeSet::StaticClass();
	ULomEquipmentItemInstance* FirstItem = NewObject<ULomEquipmentItemInstance>(FirstPlayer);
	ULomEquipmentItemInstance* SecondItem = NewObject<ULomEquipmentItemInstance>(SecondPlayer);
	ULomEquipmentItemInstance* OtherFirstItem = NewObject<ULomEquipmentItemInstance>(FirstPlayer);

	for (int32 Cycle = 0; Cycle < 3; ++Cycle)
	{
		FirstItem->AddAbilitySet(SharedSet, FirstPlayer);
		SecondItem->AddAbilitySet(SharedSet, SecondPlayer);
		OtherFirstItem->AddAbilitySet(SharedSet, FirstPlayer);
		TestEqual(TEXT("First player has two independent grants"), FirstASC->GetActivatableAbilities().Num(), 2);
		TestEqual(TEXT("Second player has one grant"), SecondASC->GetActivatableAbilities().Num(), 1);

		FirstItem->RemoveAbilitySet(FirstPlayer);
		FirstItem->RemoveAbilitySet(FirstPlayer);
		TestEqual(TEXT("Repeated removal preserves another item on the same player"), FirstASC->GetActivatableAbilities().Num(), 1);
		TestEqual(TEXT("Removal preserves the other player's ability"), SecondASC->GetActivatableAbilities().Num(), 1);
		TestEqual(TEXT("Removal preserves the other player's attribute set"), SecondASC->GetSpawnedAttributes().Num(), 1);

		SecondItem->RemoveAbilitySet(SecondPlayer);
		OtherFirstItem->RemoveAbilitySet(FirstPlayer);
		TestEqual(TEXT("First player has no leftover abilities"), FirstASC->GetActivatableAbilities().Num(), 0);
		TestEqual(TEXT("Second player has no leftover abilities"), SecondASC->GetActivatableAbilities().Num(), 0);
		TestEqual(TEXT("First player has no leftover attributes"), FirstASC->GetSpawnedAttributes().Num(), 0);
		TestEqual(TEXT("Second player has no leftover attributes"), SecondASC->GetSpawnedAttributes().Num(), 0);
	}
	World->DestroyWorld(false);
	return true;
}

#endif
