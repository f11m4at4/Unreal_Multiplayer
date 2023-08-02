// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKTEST_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UNetGameInstance();

	virtual void Init() override;
public:
	UPROPERTY()
	FString mySessionName;

	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	UFUNCTION(BlueprintCallable)
	void CreateMySession(FText roomName, int32 playercount);

	void OnCreateMySession(FName sessionName, bool bWasSuccessful);

	void FindOtherSession();
};
