// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "card.h"
#include "Components/TimelineComponent.h"
#include "cardActor.generated.h"

UCLASS()
class CARDGAME_API AcardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcardActor(); //FString UID, FString name, float hp, float damage

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	void setupCard(FString UID, FString name, float hp, float damage);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* root;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* cardWidget;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* hologramWidget;

	UPROPERTY(EditAnywhere)
	class UDataTable* cardDatatable;
	UPROPERTY(EditAnywhere)
	class UDataTable* hologramDatatable;

	// card variables
	FString UID;
	FString name;
	int hp;
	int bonusHp;
	int damage;
	int bonusDamage;

private:
	Acard* controlledPawn;
	FVector camPos;
	
	// timeline
	FTimeline timeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* curve;
	UFUNCTION()
	void timelineUpdate(float alpha);
};
