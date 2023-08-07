// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnershipActor.h"
#include "../NetworkTestCharacter.h"
#include <EngineUtils.h>

// Sets default values
AOwnershipActor::AOwnershipActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;
	meshComp->SetWorldScale3D(FVector(0.5f));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AOwnershipActor::BeginPlay()
{
	Super::BeginPlay();
	
	localRole = GetLocalRole();
	remoteRole = GetRemoteRole();
}

// Called every frame
void AOwnershipActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintLog();

	if (HasAuthority())
	{
		AActor* newOwner = nullptr;
		float minDistance = searchDistance;

		for (TActorIterator<ANetworkTestCharacter> it(GetWorld()); it; ++it)
		{
			float currentDistance = GetDistanceTo(*it);

			if (currentDistance < minDistance)
			{
				newOwner = *it;
				minDistance = currentDistance;
			}
		}

		if (GetOwner() != newOwner)
		{
			SetOwner(newOwner);
		}

	}
	DrawDebugSphere(GetWorld(), GetActorLocation(), searchDistance, 30, FColor::Yellow, false, -1, 0, 1);
	
}

void AOwnershipActor::PrintLog()
{
	const FString localRoleString = UEnum::GetValueAsString(localRole);
	const FString remotelRoleString = UEnum::GetValueAsString(remoteRole);
	const FString ownerString = GetOwner() != nullptr ? GetOwner()->GetName() : FString("No Owner");
	const FString connectionString = GetNetConnection() != nullptr ? FString("Valid Connection") : FString("Invalid Connection");
	const FString myString = FString::Printf(TEXT("Local Role: %s\nRemote role: %s\nOwner: %s\nConnection: %s"), *localRoleString, *remotelRoleString, *ownerString, *connectionString);

	DrawDebugString(GetWorld(), GetActorLocation(), myString, nullptr, FColor::Yellow, 0, true);
}

