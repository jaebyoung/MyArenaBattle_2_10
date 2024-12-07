// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMaxHp(float NewMaxHp) 
	{
		UE_LOG(LogTemp, Warning, TEXT("UABHpBarWidget::SetMaxHp Called."));
		MaxHp = NewMaxHp; 
		UE_LOG(LogTemp, Warning, TEXT("UABHpBarWidget::SetMaxHp Called."));
	}
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
