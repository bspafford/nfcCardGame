// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "playerHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARDGAME_API UplayerHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, meta = (Bindwidget))
	class UCanvasPanel* canvas;

	// p1
	UPROPERTY(EditAnywhere, meta = (Bindwidget))
	class UImage* player1Icon;
	UPROPERTY(EditAnywhere, meta = (Bindwidget))
	class UTextBlock* player1Health;

	// p2
	UPROPERTY(EditAnywhere, meta = (Bindwidget))
	class UImage* player2Icon;
	UPROPERTY(EditAnywhere, meta = (Bindwidget))
	class UTextBlock* player2Health;
};
