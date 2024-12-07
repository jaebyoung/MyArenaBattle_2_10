// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "ABGameMode.h"
#include "Engine/AssetManager.h"


AABGameMode::AABGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("AABGameMode::AABGameMode Called."));
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	 
	UE_LOG(LogTemp, Warning, TEXT("AABGameMode::AABGameMode Ended."));
}

void AABGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("AABGameMode::InitGame Called."));
	Super::InitGame(MapName, Options, ErrorMessage);

	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	/*ensure(0 < Assets.Num());*/
	if (Assets.Num() > 0)
	{
		for (const auto& Asset : Assets)
		{
			FString AssetType = Asset.PrimaryAssetType.ToString();
			FString AssetName = Asset.PrimaryAssetName.ToString();

			UE_LOG(LogTemp, Log, TEXT("Asset ID: Type = %s, Name = %s"), *AssetType, *AssetName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Assets is NULL"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("AABGameMode::InitGame Ended."));
}
