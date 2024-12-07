// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	UE_LOG(LogTemp, Warning, TEXT("UABGameSingleton::UABGameSingleton Called."));
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable'"));
	if (DataTableRef.Object != nullptr)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		const TMap<FName, uint8*>& RowMap = DataTable->GetRowMap();

		for (const auto& Row : RowMap)
		{
			const FName& RowName = Row.Key;
			const uint8* RowDataPtr = Row.Value;

			const FABCharacterStat* RowData = reinterpret_cast<const FABCharacterStat*>(RowDataPtr);
		
			if (RowData)
			{
				UE_LOG(LogTemp, Warning, TEXT("RowName: %s, MaxHp: %f, Attack: %f, AttackRange: %f, AttackSpeed: %f, MovementSpeed: %f"),
					*RowName.ToString(),
					RowData->MaxHp,
					RowData->AttackDamage,
					RowData->AttackRange,
					RowData->AttackSpeed,
					RowData->MovementSpeed
				);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("RowName: %s has invalid data"), *RowName.ToString());
			}
		
		}

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
		UE_LOG(LogTemp, Warning, TEXT("UABGameSingleton::UABGameSingleton Ended."));
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton& UABGameSingleton::Get()
{
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UABGameSingleton>();

}
