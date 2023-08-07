// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolActor.h"
#include <Components/BoxComponent.h>

// Sets default values
APistolActor::APistolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetSimulatePhysics(true);
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APistolActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

