// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "cardWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARDGAME_API UcardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* canvas;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* thumbnail;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* name;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* health;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* damage;
};
