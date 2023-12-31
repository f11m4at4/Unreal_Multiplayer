// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKTEST_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MySettings")
	bool bHasPistol = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MySettings")
	float degree = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MySettings")
	float speed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MySettings")
	bool bIsStart = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MySettings")
	float deltaRot;

private:
	UPROPERTY()
	class ANetworkTestCharacter* player;

	UPROPERTY()
	class UCharacterMovementComponent* movementComp;

	float CaculateDir(FVector velocity, FRotator rot);

};
