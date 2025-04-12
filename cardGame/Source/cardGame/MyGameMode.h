// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "card.h"
#include "Components/TimelineComponent.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CARDGAME_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	void changeTurns();
public:
	void playerJoined();
	void checkBattle(Acard* playerRef);
	UFUNCTION()
	void shouldAttackSlot(bool slot1, bool slot2, bool slot3, bool slot4, bool slot5);
	int numRecievedSlots;

	TArray<int> attackSlots;
	
	bool player1Went;
	bool player2Went;

	int currentPlayersTurn = 0;
private:

	Acard* player1; // might need to cast to it for reference?
	Acard* player2;

	FText oppsTurn = FText::FromString("Opponents Turn");
	FText yourTurn = FText::FromString("Your Turn");

	void setTurnText();

	// attack Timeline
	FTimeline attackTimeline;
	UPROPERTY(EditAnywhere);
	UCurveVector* attackingCurve;

	UFUNCTION()
	void attackUpdate(FVector alpha);
	UFUNCTION()
	void startNewRound();

	UFUNCTION()
	void attack();
	UFUNCTION()
	void attackFinished();
};
