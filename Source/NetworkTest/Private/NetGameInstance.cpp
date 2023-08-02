// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
#include "OnlineSessionSettings.h"

UNetGameInstance::UNetGameInstance()
{
	mySessionName = TEXT("205");
}

void UNetGameInstance::Init()
{
	Super::Init();

	if (auto subsys = IOnlineSubsystem::Get())
	{
		sessionInterface = subsys->GetSessionInterface();
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateMySession);

		//FTimerHandle timerHandle;
		//GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UNetGameInstance::CreateMySession, 1, false);
	}
}

void UNetGameInstance::CreateMySession(FText roomName, int32 playercount)
{
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == TEXT("NULL");

	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.NumPublicConnections = playercount;

	bool isSuccess = sessionInterface->CreateSession(0, FName(roomName.ToString()), sessionSettings);
	UE_LOG(LogTemp, Warning, TEXT("Session Create Result : %s"), isSuccess ? TEXT("Success!!") : TEXT("Failed!!"));
}

void UNetGameInstance::OnCreateMySession(FName sessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateMySession"));
		GetWorld()->ServerTravel(TEXT("/Game/Maps/BattleMap?listen"));
	}
}

void UNetGameInstance::FindOtherSession()
{
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == TEXT("NULL");
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	sessionSearch->MaxSearchResults = 10;
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}
