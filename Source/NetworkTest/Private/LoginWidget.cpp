// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "NetGameInstance.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/Slider.h>
#include <UMG/Public/Components/EditableText.h>
#include <UMG/Public/Components/TextBlock.h>

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = GetGameInstance<UNetGameInstance>();
	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickCreateButton);
	slider_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnSliderMoved);
}

void ULoginWidget::OnClickCreateButton()
{
	if (gi != nullptr && !edit_roomName->GetText().IsEmpty())
	{
		gi->CreateMySession(edit_roomName->GetText(), (int32)slider_playerCount->GetValue());
	}
}

void ULoginWidget::OnSliderMoved(float Value)
{
	text_sliderCount->SetText(FText::AsNumber((int32)Value));
}
