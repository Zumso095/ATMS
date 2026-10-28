#include "UI/LomLoadingScreenSubsystem.h"
#include "Framework/Application/IInputProcessor.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/WorldSettings.h"
#include "ShaderPipelineCache.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"

class FLoadingScreenInputPreProcessor : public IInputProcessor
{
public:
	FLoadingScreenInputPreProcessor() {}
	virtual ~FLoadingScreenInputPreProcessor() {}

	bool CanEatInput() const
	{
		return !GIsEditor;
	}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return CanEatInput(); }
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return CanEatInput(); }
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override { return CanEatInput(); }
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override { return CanEatInput(); }
	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override { return CanEatInput(); }
};

void ULomLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::HandlePostLoadMap);
	FWorldDelegates::OnSeamlessTravelStart.AddUObject(this, &ThisClass::HandleSeamlessTravel);

	UGameInstance* GameInstance = GetGameInstance();
	check(GameInstance);
}

void ULomLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	FWorldDelegates::OnSeamlessTravelStart.RemoveAll(this);

	HideLoadingScreen();
	SetTickableTickType(ETickableTickType::Never);
}

bool ULomLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer);
	const bool bIsServerWorld = GameInstance->IsDedicatedServerInstance();
	return !bIsServerWorld;
}

void ULomLoadingScreenSubsystem::Tick(float DeltaTime)
{
	UpdateLoadingScreen();
}

ETickableTickType ULomLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}
	return ETickableTickType::Conditional;
}

bool ULomLoadingScreenSubsystem::IsTickable() const
{
	UGameInstance* GameInstance = GetGameInstance();
	return (GameInstance && GameInstance->GetGameViewportClient());
}

TStatId ULomLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULomLoadingScreenSubsystem, STATGROUP_Tickables);
}

UWorld* ULomLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	return GetGameInstance()->GetWorld();
}

void ULomLoadingScreenSubsystem::PrepareLoadingWidget(FString MapName)
{
	LoadingMapName = MapName;
	PreLoadMapInfo(MapName);
}

void ULomLoadingScreenSubsystem::HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance == GetGameInstance())
	{
		bCurrentlyInLoadMap = true;
		bSeamlessTravel = false;
		LoadingMapName = MapName;

		const bool bWasStartupLoading = bStartupLoading;
		PreLoadMapInfo(MapName);

		// Если экран уже был показан ранее (например, по TravelURL), но тип виджета сменился — пересоздаем его корректно
		if (bCurrentlyShowingLoadingScreen && (bWasStartupLoading != bStartupLoading))
		{
			HideLoadingScreen();
		}

		if (GEngine->IsInitialized())
		{
			UpdateLoadingScreen();
		}
	}
}

void ULomLoadingScreenSubsystem::HandlePostLoadMap(UWorld* World)
{
	if ((World != nullptr) && (World->GetGameInstance() == GetGameInstance()))
	{
		bCurrentlyInLoadMap = false;
		LoadingMapName.Empty();
		UpdateLoadingScreen();
	}
}

void ULomLoadingScreenSubsystem::HandleSeamlessTravel(UWorld* World, const FString& MapName)
{
	if (World && World->GetGameInstance() == GetGameInstance())
	{
		// Не выставляем bCurrentlyInLoadMap = true, так как при Seamless Travel не вызывается PostLoadMapWithWorld.
		// Состояние бесшовного перехода отслеживается через World->IsInSeamlessTravel().
		bSeamlessTravel = true;
		LoadingMapName = MapName;
		PreLoadMapInfo(MapName);

		if (GEngine->IsInitialized())
		{
			UpdateLoadingScreen();
		}
	}
}

void ULomLoadingScreenSubsystem::PreLoadMapInfo(FString MapName)
{
	const ULomLoadingScreenSettings* Settings = GetDefault<ULomLoadingScreenSettings>();

	if (MapName.IsEmpty())
	{
		if (const UGameInstance* LocalGI = GetGameInstance())
		{
			if (const UWorld* World = LocalGI->GetWorld())
			{
				MapName = World->GetMapName();
			}
		}
	}

	const FString MenuName = Settings->MenuLevelName.ToString();
	if (!MenuName.IsEmpty() && !MenuName.Equals(TEXT("None"), ESearchCase::IgnoreCase) && MapName.Contains(MenuName))
	{
		bStartupLoading = true;
	}
	else
	{
		bStartupLoading = false;
	}
}

void ULomLoadingScreenSubsystem::PrepareMoviePlayer()
{
}

void ULomLoadingScreenSubsystem::UpdateLoadingScreen()
{
	if (ShouldShowLoadingScreen())
	{
		ShowLoadingScreen();
	}
	else
	{
		HideLoadingScreen();
	}
}

void ULomLoadingScreenSubsystem::ShowLoadingScreen()
{
	if (bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	TimeLoadingScreenShown = FPlatformTime::Seconds();

	const ULomLoadingScreenSettings* Settings = GetDefault<ULomLoadingScreenSettings>();
	UGameInstance* LocalGameInstance = GetGameInstance();
	if (!LocalGameInstance)
	{
		return;
	}

	UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient();
	if (!GameViewportClient)
	{
		return;
	}

	PreLoadMapInfo(LoadingMapName);

	if (!UserWidget)
	{
		const FSoftClassPath WidgetClassPath = bStartupLoading ? Settings->StartupWidget : Settings->LoadingScreenWidget;
		TSubclassOf<UUserWidget> LoadingWidgetClass = WidgetClassPath.TryLoadClass<UUserWidget>();
		if (LoadingWidgetClass)
		{
			UserWidget = UUserWidget::CreateWidgetInstance(*LocalGameInstance, LoadingWidgetClass, NAME_None);
		}
	}

	if (!UserWidget)
	{
		return;
	}

	bCurrentlyShowingLoadingScreen = true;
	LoadingScreenWidget = UserWidget->TakeWidget();

	GameViewportClient->AddViewportWidgetContent(LoadingScreenWidget.ToSharedRef(), Settings->LoadingScreenZOrder);

	StartBlockingInput();
	ChangePerformanceSettings(true);

	if (!GIsEditor || Settings->ForceTickLoadingScreenEvenInEditor)
	{
		FSlateApplication::Get().Tick();
	}
}

void ULomLoadingScreenSubsystem::HideLoadingScreen()
{
	if (!bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	RemoveWidgetFromViewport();
	StopBlockingInput();
	ChangePerformanceSettings(false);

	LoadingMapName.Empty();
	bSeamlessTravel = false;
	bCurrentlyShowingLoadingScreen = false;
}

void ULomLoadingScreenSubsystem::RemoveWidgetFromViewport()
{
	UGameInstance* LocalGameInstance = GetGameInstance();
	if (LoadingScreenWidget.IsValid())
	{
		if (LocalGameInstance)
		{
			if (UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient())
			{
				GameViewportClient->RemoveViewportWidgetContent(LoadingScreenWidget.ToSharedRef());
			}
		}
		LoadingScreenWidget.Reset();
	}
	UserWidget = nullptr;
}

bool ULomLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const ULomLoadingScreenSettings* Settings = GetDefault<ULomLoadingScreenSettings>();

#if !UE_BUILD_SHIPPING
	static bool bCmdLineNoLoadingScreen = FParse::Param(FCommandLine::Get(), TEXT("NoLoadingScreen"));
	if (bCmdLineNoLoadingScreen)
	{
		return false;
	}
#endif

	UGameInstance* LocalGameInstance = GetGameInstance();
	if (!LocalGameInstance || LocalGameInstance->GetGameViewportClient() == nullptr)
	{
		return false;
	}

	const bool bNeedToShowLoadingScreen = CheckForAnyNeedToShowLoadingScreen();

	bool bWantToForceShowLoadingScreen = false;
	if (bNeedToShowLoadingScreen)
	{
		TimeLoadingScreenLastDismissed = -1.0;
	}
	else
	{
		const double CurrentTime = FPlatformTime::Seconds();
		const bool bCanHoldLoadingScreen = (!GIsEditor || Settings->HoldLoadingScreenAdditionalSecsEvenInEditor);
		const double HoldLoadingScreenAdditionalSecs = bCanHoldLoadingScreen ? Settings->HoldLoadingScreenAdditionalSecs : 0.0;

		if (TimeLoadingScreenLastDismissed < 0.0)
		{
			TimeLoadingScreenLastDismissed = CurrentTime;
		}
		const double TimeSinceScreenDismissed = CurrentTime - TimeLoadingScreenLastDismissed;

		if ((HoldLoadingScreenAdditionalSecs > 0.0) && (TimeSinceScreenDismissed < HoldLoadingScreenAdditionalSecs))
		{
			// Включаем рендер мира заранее, чтобы за время удержания экрана подгрузились текстуры и шейдеры
			if (UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient())
			{
				GameViewportClient->bDisableWorldRendering = false;
			}
			bWantToForceShowLoadingScreen = true;
		}
	}

	return bNeedToShowLoadingScreen || bWantToForceShowLoadingScreen;
}

bool ULomLoadingScreenSubsystem::CheckForAnyNeedToShowLoadingScreen()
{
	const UGameInstance* LocalGameInstance = GetGameInstance();
	if (!LocalGameInstance)
	{
		return false;
	}

	const FWorldContext* Context = LocalGameInstance->GetWorldContext();
	if (Context == nullptr)
	{
		return true;
	}

	UWorld* World = Context->World();
	if (World == nullptr)
	{
		return true;
	}

	AGameStateBase* GameState = World->GetGameState<AGameStateBase>();
	if (GameState == nullptr)
	{
		return true;
	}

	if (bCurrentlyInLoadMap)
	{
		return true;
	}

	if (!Context->TravelURL.IsEmpty())
	{
		LoadingMapName = Context->TravelURL;
		return true;
	}

	if (Context->PendingNetGame != nullptr)
	{
		return true;
	}

	if (!World->HasBegunPlay())
	{
		return true;
	}

	if (World->IsInSeamlessTravel())
	{
		return true;
	}

	bool bFoundAnyLocalPC = false;
	for (ULocalPlayer* LP : LocalGameInstance->GetLocalPlayers())
	{
		if (LP != nullptr && LP->PlayerController != nullptr)
		{
			bFoundAnyLocalPC = true;
			break;
		}
	}

	if (!bFoundAnyLocalPC)
	{
		return true;
	}

	return false;
}

void ULomLoadingScreenSubsystem::ChangePerformanceSettings(bool bEnabingLoadingScreen)
{
	UGameInstance* LocalGameInstance = GetGameInstance();
	if (!LocalGameInstance)
	{
		return;
	}

	UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient();
	if (!GameViewportClient)
	{
		return;
	}

	FShaderPipelineCache::SetBatchMode(bEnabingLoadingScreen ? FShaderPipelineCache::BatchMode::Fast : FShaderPipelineCache::BatchMode::Background);
	GameViewportClient->bDisableWorldRendering = bEnabingLoadingScreen;

	if (UWorld* ViewportWorld = GameViewportClient->GetWorld())
	{
		if (AWorldSettings* WorldSettings = ViewportWorld->GetWorldSettings(false, false))
		{
			WorldSettings->bHighPriorityLoadingLocal = bEnabingLoadingScreen;
		}
	}
}

void ULomLoadingScreenSubsystem::StartBlockingInput()
{
	if (!InputPreProcessor.IsValid())
	{
		InputPreProcessor = MakeShareable<FLoadingScreenInputPreProcessor>(new FLoadingScreenInputPreProcessor());
		FSlateApplication::Get().RegisterInputPreProcessor(InputPreProcessor, 0);
	}
}

void ULomLoadingScreenSubsystem::StopBlockingInput()
{
	if (InputPreProcessor.IsValid())
	{
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(InputPreProcessor);
		}
		InputPreProcessor.Reset();
	}
}