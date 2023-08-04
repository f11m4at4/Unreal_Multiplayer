// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionSlotInfo
{
	GENERATED_USTRUCT_BODY()

	FString roomName = TEXT("");
	FString hostName = TEXT("");
	FString playerCount = TEXT("");
	int32 pingSpeed = 0;
	int32 sessionIndex = 0;

	FORCEINLINE void Set(FString rn, FString hn, FString pCount, int32 ping, int32 index)
	{
		roomName = rn;
		hostName = hn;
		playerCount = pCount;
		pingSpeed = ping;
		sessionIndex = index;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, FSessionSlotInfo, sessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindButtonSignature, bool, bIsActivation);

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
	FSearchSignature onSearchCompleted;
	FFindButtonSignature onFindButtonActivation;

	UFUNCTION(BlueprintCallable)
	void CreateMySession(FText roomName, int32 playercount);

	void OnCreateMySession(FName sessionName, bool bWasSuccessful);

	void FindOtherSession();

	void OnFindOtherSessions(bool bWasSuccessful);

	void JoinSelectedSession(int32 index);

	void OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result);
};
