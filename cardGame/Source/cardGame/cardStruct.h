// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "cardStruct.generated.h"

USTRUCT(BlueprintType)
struct FcardStruct : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UTexture2D* thumbnail;
};
