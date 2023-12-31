// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"




UCLASS()
class NETWORKTEST_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UEditableText* edit_roomName;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class USlider* slider_playerCount;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UTextBlock* text_sliderCount;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_CreateSession;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UScrollBox* sb_RoomListBox;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_FindSession;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UWidgetSwitcher* ws_SessionUISwitch;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_CreateSelection;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_FindSelection;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_BackFromCreate;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_BackFromFind;

	UPROPERTY(EditDefaultsOnly, Category=MySettings, BlueprintReadWrite)
	TSubclassOf<class USessionInfoWidget> sessionInfoWidget;

	UPROPERTY()
	class UNetGameInstance* gi;
	UPROPERTY()
	class USessionInfoWidget* sessionSlot;


	UFUNCTION()
	void OnClickCreateButton();
	UFUNCTION()
	void OnClickFindButton();


	UFUNCTION()
	void OnSliderMoved(float Value);

	UFUNCTION()
	void OnClickedCreateSelection();

	UFUNCTION()
	void OnClickedFindSelection();

	UFUNCTION()
	void AddRoomSlot(struct FSessionSlotInfo slotInfo);

	UFUNCTION()
	void ChangeButtonActivation(bool bIsActivation);

	UFUNCTION()
	void BackToFirstCanvas();

private:
	void SwitchCanvas(int32 index);
};
