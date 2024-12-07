// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::UABCharacterStatComponent Called."));
	
	CurrentLevel = 1;
	//MaxHp = 250.0f;
	//CurrentHp = MaxHp;


	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::UABCharacterStatComponent Ended."));
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::BeginPlay Called."));
	//SetHp(MaxHp);

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);

	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::BeginPlay Ended."));
	
}


void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::SetLevelStat Called."));
	
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp >0.0f);

	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::SetLevelStat Ended."));
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::SetHp Called."));

	/*UE_LOG(LogTemp, Warning, TEXT("Beginplay : SetHp"));*/
	CurrentHp = FMath::Clamp<float>(NewHp, 0, BaseStat.MaxHp);

	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::SetHp's OnHpChanged.Broadcast Called."));
	OnHpChanged.Broadcast(CurrentHp);
	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::SetHp's OnHpChanged.Broadcast Ended."));

	UE_LOG(LogTemp, Warning, TEXT("UABCharacterStatComponent::SetHp Ended."));

}


// Called every frame
void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
