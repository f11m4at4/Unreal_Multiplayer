// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OwnershipActor.generated.h"

UCLASS()
class NETWORKTEST_API AOwnershipActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOwnershipActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PrintLog();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category="Net")
	float searchDistance = 500;

private:
	enum ENetRole localRole;
	enum ENetRole remoteRole;

};
