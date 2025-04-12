// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "hologramStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FhologramStruct : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* thumbnail;
};