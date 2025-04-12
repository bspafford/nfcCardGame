// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/WidgetComponent.h"

#include "board.generated.h"

UCLASS()
class CARDGAME_API Aboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* root;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* plane;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder11;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder12;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder13;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder14;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder15;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder21;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder22;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder23;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder24;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardBorder25;
};
