// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
#include "OnlineSessionSettings.h"
#include "LoginWidget.h"

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
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindOtherSessions);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnJoinSelectedSession);
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
	sessionSettings.Set(TEXT("ROOM_NAME"), roomName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(TEXT("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


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
	UE_LOG(LogTemp, Warning, TEXT("FindOtherSession"));
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == TEXT("NULL");
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	sessionSearch->MaxSearchResults = 10;
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	onFindButtonActivation.Broadcast(false);
}

void UNetGameInstance::OnFindOtherSessions(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindOtherSessions"));
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("Find Sessions: %d"), searchResults.Num());
		for (int i=0; i<searchResults.Num();i++)
		{
			auto result = searchResults[i];
			FString roomName;
			result.Session.SessionSettings.Get(TEXT("ROOM_NAME"), roomName);

			FString hostName;
			result.Session.SessionSettings.Get(TEXT("HOST_NAME"), hostName);
			int32 openNumber = result.Session.NumOpenPublicConnections;
			int32 maxNumber = result.Session.SessionSettings.NumPublicConnections;
			int32 pingSpeed = result.PingInMs;

			UE_LOG(LogTemp, Warning, TEXT("Room Name: %s, HostName: %s, OpenNumber: %d, Max Number:%d, Ping Speed:%d"), *roomName, *hostName, openNumber, maxNumber, pingSpeed);

			FSessionSlotInfo slotInfo;
			slotInfo.Set(roomName, hostName, FString::Printf(TEXT("%d/%d"), maxNumber - openNumber, maxNumber), pingSpeed, i);
			onSearchCompleted.Broadcast(slotInfo);
		}
		onFindButtonActivation.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search failed!!"));
	}
}

void UNetGameInstance::JoinSelectedSession(int32 index)
{
	sessionInterface->JoinSession(0, *mySessionName, sessionSearch->SearchResults[index]);
}

void UNetGameInstance::OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnJoinSelectedSession Result : %s, Session Name : %s"), result == EOnJoinSessionCompleteResult::Success?TEXT("Success"): TEXT("Failed!!"), sessionName);
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSelectedSession"));
	switch (result)
	{
	case EOnJoinSessionCompleteResult::Success:
	{
		UE_LOG(LogTemp, Warning, TEXT("Success %s"), *sessionName.ToString());

		auto playerCon = GetWorld()->GetFirstPlayerController();
		if (playerCon != nullptr)
		{
			FString url;
			sessionInterface->GetResolvedConnectString(sessionName, url);
			UE_LOG(LogTemp, Warning, TEXT("-------- Connention URL : %s"), *url);
			if (url.IsEmpty() == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("ClientTravel"));
				playerCon->ClientTravel(url, ETravelType::TRAVEL_Absolute);
			}
		}
	} 
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Warning, TEXT("SessionIsFull"));
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Warning, TEXT("SessionDoesNotExist"));
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogTemp, Warning, TEXT("CouldNotRetrieveAddress"));
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogTemp, Warning, TEXT("AlreadyInSession"));
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogTemp, Warning, TEXT("UnknownError"));
		break;
	default:
		break;
	}

}
