// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

UABWidgetComponent::UABWidgetComponent()
{
	
	UE_LOG(LogTemp, Warning, TEXT("UABWidgetComponent::UABWidgetComponent called."));
	UE_LOG(LogTemp, Warning, TEXT("UABWidgetComponent::UABWidgetComponent ended."));
}

void UABWidgetComponent::InitWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("UABWidgetComponent::InitWidget Called."));
	if (GetWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("Before Super::InitWidget(), Has Widget"));
	}
	else if (!GetWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("Before Super::InitWidget(), Has no Widget"));
	}





	UE_LOG(LogTemp, Warning, TEXT("Super::InitWidget() Called."));

	Super::InitWidget();

	UE_LOG(LogTemp, Warning, TEXT("Super::InitWidget() Ended."));






	if (GetWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("After Super::InitWidget() = Has Widget"));
	}
	else if (!GetWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("After Super::InitWidget() = Has no Widget"));
	}






	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}


	UE_LOG(LogTemp, Warning, TEXT("UABWidgetComponent::InitWidget Ended."));
}
