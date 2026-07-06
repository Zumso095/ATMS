// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicEntryBoxBase.h"
#include "GameplayTags.h"
#include "LomUIAnchorPoint.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomUIAnchorPoint : public UDynamicEntryBoxBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anchor Settings")
	FGameplayTag PointTag;

	UFUNCTION(BlueprintCallable)
	UUserWidget* AddChild(TSubclassOf<UUserWidget> WidgetClass);

	void RemoveChild(UUserWidget* Widget);

	void RegisterAnchor();
	void ResetAnchor();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
public: 
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

};
