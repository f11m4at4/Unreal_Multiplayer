// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "../NetworkTestCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	player = Cast<ANetworkTestCharacter>(GetOwningActor());
	if (player != nullptr)
	{
		movementComp = player->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (movementComp)
	{
		speed = movementComp->Velocity.Size2D();
		degree = CaculateDir(movementComp->Velocity, player->GetActorRotation());
		FRotator delta = (player->GetActorRotation() - player->GetBaseAimRotation()).GetNormalized();
		deltaRot = delta.Pitch;
	}

}

float UPlayerAnimInstance::CaculateDir(FVector velocity, FRotator rot)
{
	if (velocity.IsNearlyZero())
	{
		return 0.0f;
	}
	FVector forwardVector = FRotationMatrix(rot).GetUnitAxis(EAxis::X);
	FVector rightVector = FRotationMatrix(rot).GetUnitAxis(EAxis::Y);
	FVector speedVector = velocity.GetSafeNormal2D();

	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(forwardVector, speedVector)));

	float rightDot = FVector::DotProduct(rightVector, speedVector);

	if (rightDot < 0)
	{
		angle *= -1;
	}

	return angle;
}
