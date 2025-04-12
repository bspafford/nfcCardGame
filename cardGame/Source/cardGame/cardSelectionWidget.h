// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "cardSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARDGAME_API UcardSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UCanvasPanel* canvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UHorizontalBox* horizontalBox;

	// slot 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UOverlay* slot1Overlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UImage* slot1Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* slot1Text;

	// slot 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UOverlay* slot2Overlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UImage* slot2Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* slot2Text;

	// slot 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UOverlay* slot3Overlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UImage* slot3Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* slot3Text;

	// slot 4
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UOverlay* slot4Overlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UImage* slot4Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* slot4Text;

	// slot 5
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UOverlay* slot5Overlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UImage* slot5Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* slot5Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UTextBlock* turnText;
};
