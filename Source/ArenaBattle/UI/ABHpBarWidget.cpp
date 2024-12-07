// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	UE_LOG(LogTemp, Warning, TEXT("UABHpBarWidget::UABHpBarWidget Called."));
	MaxHp = -1.0f;

	UE_LOG(LogTemp, Warning, TEXT("UABHpBarWidget::UABHpBarWidget Ended."));
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	UE_LOG(LogTemp, Warning, TEXT("UABHpBarWidget::UpdateHpBar Called."));

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}													

}

void UABHpBarWidget::NativeConstruct()
{

	UE_LOG(LogTemp, Warning, TEXT("UABHpBarWidget::NativeConstruct Called."));

	Super::NativeConstruct();



	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));

	ensure(HpProgressBar);


	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

	UE_LOG(LogTemp, Warning, TEXT("UABHpBarWidget::NativeConstruct Ended."));
}
