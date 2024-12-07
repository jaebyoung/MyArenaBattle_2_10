// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTestUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UMyTestUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMyTestUserWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	void InitializeHpBar();
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UProgressBar> PbHpBar;

	//UPROPERTY()
	//TObjectPtr<class UCanvasPanel> RootCanvas;

protected:
	void CreateHpBar();
	
};
