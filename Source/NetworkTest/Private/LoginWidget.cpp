// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "NetGameInstance.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/Slider.h>
#include <UMG/Public/Components/EditableText.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/WidgetSwitcher.h>
#include "SessionInfoWidget.h"
#include <UMG/Public/Components/ScrollBox.h>

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = GetGameInstance<UNetGameInstance>();
	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickCreateButton);
	slider_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnSliderMoved);
	btn_CreateSelection->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedCreateSelection);
	btn_FindSelection->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedFindSelection);
	btn_FindSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickFindButton);
	btn_BackFromCreate->OnClicked.AddDynamic(this, &ULoginWidget::BackToFirstCanvas);
	btn_BackFromFind->OnClicked.AddDynamic(this, &ULoginWidget::BackToFirstCanvas);

	if (gi != nullptr)
	{
		gi->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddRoomSlot);
		gi->onFindButtonActivation.AddDynamic(this, &ULoginWidget::ChangeButtonActivation);
	}

}

void ULoginWidget::OnClickCreateButton()
{
	if (gi != nullptr && !edit_roomName->GetText().IsEmpty())
	{
		gi->CreateMySession(edit_roomName->GetText(), (int32)slider_playerCount->GetValue());
	}
}

void ULoginWidget::OnClickFindButton()
{
	if (gi != nullptr)
	{
		gi->FindOtherSession();
		sb_RoomListBox->ClearChildren();
		
	}
}

void ULoginWidget::OnSliderMoved(float Value)
{
	text_sliderCount->SetText(FText::AsNumber((int32)Value));
}

void ULoginWidget::OnClickedCreateSelection()
{
	SwitchCanvas(1);
}

void ULoginWidget::OnClickedFindSelection()
{
	UE_LOG(LogTemp, Warning, TEXT("OnClickedFindSelection %p"), ws_SessionUISwitch);
	SwitchCanvas(2);
	OnClickFindButton();
}

void ULoginWidget::AddRoomSlot(FSessionSlotInfo slotInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("AddRoomSlot"));
	sessionSlot = CreateWidget<USessionInfoWidget>(GetWorld(), sessionInfoWidget);

	if (sessionSlot != nullptr)
	{
		sessionSlot->text_RoomName->SetText(FText::FromString(slotInfo.roomName));
		sessionSlot->text_HostName->SetText(FText::FromString(slotInfo.hostName));
		sessionSlot->text_PlayerCount->SetText(FText::FromString(slotInfo.playerCount));
		sessionSlot->text_PingSpeed->SetText(FText::AsNumber(slotInfo.pingSpeed));
		sessionSlot->sessionIndex = slotInfo.sessionIndex;

		sb_RoomListBox->AddChild(sessionSlot);
	}
}

void ULoginWidget::ChangeButtonActivation(bool bIsActivation)
{
	btn_FindSession->SetIsEnabled(bIsActivation);
}

void ULoginWidget::BackToFirstCanvas()
{
	ws_SessionUISwitch->SetActiveWidgetIndex(0);
}

void ULoginWidget::SwitchCanvas(int32 index)
{
	ws_SessionUISwitch->SetActiveWidgetIndex(index);
}
