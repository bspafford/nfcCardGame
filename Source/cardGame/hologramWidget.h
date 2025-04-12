// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "hologramWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARDGAME_API UhologramWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(Bindwidget))
	class UCanvasPanel* canvas;
	
	UPROPERTY(EditAnywhere, meta=(Bindwidget))
	class UImage* hologramImage;
	UPROPERTY(EditAnywhere, meta=(Bindwidget))
	class UVerticalBox* verticalBox;
	UPROPERTY(EditAnywhere, meta=(Bindwidget))
	class UTextBlock* hologramHealth;
	UPROPERTY(EditAnywhere, meta=(Bindwidget))
	class UTextBlock* hologramDamage;
};
