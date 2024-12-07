// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Engine/OverlapResult.h"
#include "Item/ABItemBox.h"


// Sets default values
AABStageGimmick::AABStageGimmick()
{
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);


	//Gate Section
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));

	for (FName GateSocket : GateSockets)
	{
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket); 
		Gate->SetStaticMesh(GateMeshRef.Object);
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
		Gates.Add(GateSocket, Gate);

		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);  
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket); 
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
		GateTrigger->ComponentTags.Add(GateSocket);

		GateTriggers.Add(GateTrigger);

	}
	//State Section
	CurrentState = EStageState::READY;

	StageChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady)));
	StageChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight)));
	StageChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward)));
	StageChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext)));

	//Fight Section
	OpponentSpawnTime = 2.f;
	OpponentClass = AABCharacterNonPlayer::StaticClass();

	//Reward Section
	RewardBoxClass = AABItemBox::StaticClass();
	for (FName GateSocket : GateSockets)
	{
		RewardBoxLocations.Add(GateSocket, Stage->GetSocketLocation(GateSocket) / 2);
	}

	CurrentStageNum = 0;
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnStageTriggerBeginOverlap called"));
	SetState(EStageState::FIGHT);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnGateTriggerBeginOverlap called"));
	UE_LOG(LogTemp, Warning, TEXT("OverlappedComponent's Name : %s"), *OverlappedComponent->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Overlapped OtherActor Name : %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Overlapped OtherComp Name : %s"), *OtherComp->GetName());

	check(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	check(Stage->DoesSocketExist(SocketName));

	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllStaticObjects, 
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam

	);

#if ENABLE_DRAW_DEBUG

	DrawDebugSphere(
		GetWorld(),
		NewLocation,
		775.0f,
		12,
		FColor::Blue,
		false,
		5.0f,
		0,
		2.0f
	);
#endif

	if (!bResult)
	{
		AABStageGimmick* NewStage = GetWorld()->SpawnActor<AABStageGimmick>(NewLocation, FRotator::ZeroRotator);
		if (NewStage)
		{
			NewStage->SetStageNum(CurrentStageNum + 1);
		}
	}


}

void AABStageGimmick::OpenAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	}
}

void AABStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;
	if (StageChangeActions.Contains(InNewState))
	{
		StageChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void AABStageGimmick::SetReady()
{
	UE_LOG(LogTemp, Warning, TEXT("SetReady Called"));

	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	
	for (auto GateTrigger : GateTriggers) 
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

}

void AABStageGimmick::SetFight()
{
	UE_LOG(LogTemp, Warning, TEXT("SetFight Called"));
	CloseAllGates();
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();


	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);

}

void AABStageGimmick::SetChooseReward()
{
	UE_LOG(LogTemp, Warning, TEXT("SetChooseReward Called"));

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates();
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	UE_LOG(LogTemp, Warning, TEXT("SetChooseNext Called"));

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}
	OpenAllGates();
}

void AABStageGimmick::OnOpponentDestroyed(AActor* DestroyActor)
{
	SetState(EStageState::REWARD);

}

void AABStageGimmick::OnOpponentSpawn()
{
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f; 

	UE_LOG(LogTemp, Error, TEXT("GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator) Called."));
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	UE_LOG(LogTemp, Error, TEXT("GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator) Ended."));
	AABCharacterNonPlayer* ABOpponentCharacter = Cast<AABCharacterNonPlayer>(OpponentActor);
	if (ABOpponentCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AABStageGimmick::OnOpponentSpawn's SetLevel Called."));
		ABOpponentCharacter->SetLevel(CurrentStageNum);
		UE_LOG(LogTemp, Warning, TEXT("AABStageGimmick::OnOpponentSpawn's SetLevel Ended."));
	

		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroyed);
	}
}

void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AABItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (OverlappedBox != ValidItemBox)
			{
				ValidItemBox->Destroy();
			}
		}
	}
	SetState(EStageState::NEXT);
}

void AABStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f);
		AActor* ItemActor = GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
		AABItemBox* RewardBoxActor = Cast<AABItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}




