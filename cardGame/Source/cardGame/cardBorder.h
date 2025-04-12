// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "cardBorder.generated.h"

/**
 * 
 */
UCLASS()
class CARDGAME_API UcardBorder : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (Bindwidget))
	class UCanvasPanel* canvas;

	UPROPERTY(EditAnywhere, meta = (Bindwidget))
	class UImage* border;
};
