#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Blueprint/WidgetTree.h"
#include "Engine/World.h"
#include "UI/LomGameUIManagerSubsystem.h"
#include "UI/Widget/LomCommonActivatableWidget.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLomAnchorWidgetLifecycleTest,
	"Lom.UI.AnchorWidgetLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FLomAnchorWidgetLifecycleTest::RunTest(const FString& Parameters)
{
	UWorld* World = NewObject<UWorld>();
	ULomGameUIManagerSubsystem* Manager = NewObject<ULomGameUIManagerSubsystem>(World);
	ULomCommonActivatableWidget* Host = CreateWidget<ULomCommonActivatableWidget>(World);
	if (!TestNotNull(TEXT("Host widget created"), Host))
	{
		return false;
	}
	const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TEXT("UI.Anchor.Crosshair"));
	const TSubclassOf<UUserWidget> WidgetClass = ULomCommonActivatableWidget::StaticClass();
	UObject* Owner = NewObject<UWidgetTree>(World);
	UObject* OtherOwner = NewObject<UWidgetTree>(World);

	const FGuid Cancelled = Manager->RequestContentToAnchor(Tag, WidgetClass);
	TestTrue(TEXT("Pending request has a handle"), Cancelled.IsValid());
	TestNull(TEXT("Pending request has no widget yet"), Manager->GetWidgetForRequest(Cancelled));
	Manager->RemoveWidgetRequest(Cancelled);
	Manager->RemoveWidgetRequest(Cancelled);

	const FGuid CancelledByOwner = Manager->RequestContentToAnchor(Tag, WidgetClass, Owner);
	Manager->RemoveWidgetsForOwner(Owner);
	const FGuid First = Manager->RequestContentToAnchor(Tag, WidgetClass, Owner);
	const FGuid Second = Manager->RequestContentToAnchor(Tag, WidgetClass, OtherOwner);
	TestTrue(TEXT("Requests for the same anchor have different handles"), First != Second);

	ULomUIAnchorPoint* Anchor = Host->WidgetTree->ConstructWidget<ULomUIAnchorPoint>();
	Anchor->PointTag = Tag;
	Anchor->TakeWidget();
	Manager->RegisterAnchorPoint(Anchor);
	TestNull(TEXT("Cancelled request stays cancelled after registration"), Manager->GetWidgetForRequest(Cancelled));
	TestNull(TEXT("Owner cancellation stays cancelled after registration"), Manager->GetWidgetForRequest(CancelledByOwner));
	TestNotNull(TEXT("First queued widget created"), Manager->GetWidgetForRequest(First));
	TestNotNull(TEXT("Second queued widget created"), Manager->GetWidgetForRequest(Second));
	TestEqual(TEXT("Neither pending request overwrites the other"), Anchor->GetNumEntries(), 2);
	Manager->LayoutAdded();
	TestEqual(TEXT("Repeated readiness does not duplicate widgets"), Anchor->GetNumEntries(), 2);

	Manager->RemoveWidgetsForOwner(Owner);
	TestNull(TEXT("Owner's live widget removed"), Manager->GetWidgetForRequest(First));
	TestNotNull(TEXT("Other owner's widget survives"), Manager->GetWidgetForRequest(Second));
	TestEqual(TEXT("Only one entry remains"), Anchor->GetNumEntries(), 1);
	UUserWidget* Widget = Manager->GetWidgetForRequest(Second);
	Manager->RemoveWidget(Widget);
	Manager->RemoveWidget(Widget);
	TestEqual(TEXT("Legacy removal is idempotent"), Anchor->GetNumEntries(), 0);
	TestNull(TEXT("Legacy removal also clears its request"), Manager->GetWidgetForRequest(Second));

	const FGuid Immediate = Manager->RequestContentToAnchor(Tag, WidgetClass, Owner);
	TestNotNull(TEXT("Ready anchor creates immediately"), Manager->GetWidgetForRequest(Immediate));
	Manager->UnregisterAnchorPoint(Anchor);
	TestNull(TEXT("Unregister clears widgets associated with the anchor"), Manager->GetWidgetForRequest(Immediate));
	TestEqual(TEXT("Unregister releases entries"), Anchor->GetNumEntries(), 0);
	Anchor->ReleaseSlateResources(true);
	return true;
}

#endif
