// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "cardActor.h"

void AMyGameMode::BeginPlay() {
	Super::BeginPlay();

	FOnTimelineVector progressUpdate;
	progressUpdate.BindUFunction(this, FName("attackUpdate"));
	attackTimeline.AddInterpVector(attackingCurve, progressUpdate);
	FOnTimelineEvent FinishedAttack;
	FinishedAttack.BindUFunction(this, FName("attackFinished"));
	FOnTimelineEvent attackEvent;
	attackEvent.BindUFunction(this, FName("attack"));
	attackTimeline.SetTimelineFinishedFunc(FinishedAttack);
	attackTimeline.AddEvent(.2f, attackEvent);

	
}

void AMyGameMode::Tick(float deltaTime) {
	attackTimeline.TickTimeline(deltaTime);

	if (numRecievedSlots >= 2) {
		numRecievedSlots = 0;
		attackTimeline.PlayFromStart();

		//startNewRound();
	}

	FString test = "attackSlots: ";
	if (attackSlots.Num() >= 1)
		test += FString::FormatAsNumber(attackSlots[0]);
	if (attackSlots.Num() >= 2)
		test += ", " + FString::FormatAsNumber(attackSlots[1]);
	if (attackSlots.Num() >= 3)
		test += ", " + FString::FormatAsNumber(attackSlots[2]);
	if (attackSlots.Num() >= 4)
		test += ", " + FString::FormatAsNumber(attackSlots[3]);
	if (attackSlots.Num() >= 5)
		test += ", " + FString::FormatAsNumber(attackSlots[4]);

	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("%s"), *test));
}

void AMyGameMode::playerJoined() {
	player1 = Cast<Acard>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	player2 = Cast<Acard>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));

	if (player1 && player2) {
		int rand = FMath::RandRange(0, 1);
		rand = 1; // temp
		if (rand) {
			player1->turn = true;
			player2->turn = false;
			player1->canPlaceCard = true;
			currentPlayersTurn = 0;
			setTurnText();
		}
		else {
			player1->turn = false;
			player2->turn = true;
			player2->canPlaceCard = true;
			currentPlayersTurn = 1;
			setTurnText();
		}

		// setup player health widget
		player1->updatePlayerHealthWidget(player1->playerHealth, player2->playerHealth);
		player2->updatePlayerHealthWidget(player2->playerHealth, player1->playerHealth);
	}
}

void AMyGameMode::changeTurns() {
	if (currentPlayersTurn == 0)
		currentPlayersTurn = 1;
	else
		currentPlayersTurn = 0;

	if (player1 && player2) {
		player1->turn = !player1->turn;
		player2->turn = !player2->turn;

		if (player1->turn)
			player1->canPlaceCard = true;
		else if (player2->turn)
			player2->canPlaceCard = true;

		setTurnText();
	}
}

void AMyGameMode::checkBattle(Acard* playerRef) {
	if (playerRef == player1) {
		player1Went = true;
		player1->canPlaceCard = false;
	}
	else if (playerRef == player2) {
		player2Went = true;
		player2->canPlaceCard = false;
	}

	if (player1Went && player2Went) {
		// make it so they can't place cards
		player1->setTurnText(FText::FromString("Battling"));
		player2->setTurnText(FText::FromString("Battling"));
		player1->test();
		player2->test();
	}
	else {
		if (player1Went == true && player2Went == false) {
			changeTurns();
		}
	}
}

// attack timeline
void AMyGameMode::attackUpdate(FVector alpha) {
	if (attackSlots.Num() > 0) {
		player1->updateLoc(alpha, attackSlots[0]);
		player2->updateLoc(alpha, attackSlots[0]);
	}
	attackTimeline;
}

// attack timeline
void AMyGameMode::attack() {
	player1->takeDamage(player2);
	player2->takeDamage(player1);
}

// attack timeline
void AMyGameMode::attackFinished() {

	if (attackSlots.Num() > 0) {
		attackSlots.RemoveAt(0);
		attackTimeline.PlayFromStart();
		return;
	}

	startNewRound();
}

void AMyGameMode::startNewRound() {
	player1Went = false;
	player2Went = false;

	if (player2->turn)
		changeTurns();
	else
		setTurnText();

	player1->isTableFull(1);
	player2->isTableFull(2);

	//checkBattle(player1);
	//checkBattle(player2);
	//changeTurns();

	if (player1->turn)
		player1->canPlaceCard = true;
	else if (player2->turn)
		player2->canPlaceCard = true;
}

void AMyGameMode::shouldAttackSlot(bool slot1, bool slot2, bool slot3, bool slot4, bool slot5) {

	if (slot1 && !attackSlots.Contains(0))
		attackSlots.Add(0);
	if (slot2 && !attackSlots.Contains(1))
		attackSlots.Add(1);
	if (slot3 && !attackSlots.Contains(2))
		attackSlots.Add(2);
	if (slot4 && !attackSlots.Contains(3))
		attackSlots.Add(3);
	if (slot5 && !attackSlots.Contains(4))
		attackSlots.Add(4);

	numRecievedSlots++;
}

void AMyGameMode::setTurnText() {
	if (!player1 || !player2)
		return;

	if (player1->turn) {
		player1->setTurnText(yourTurn);
		player2->setTurnText(oppsTurn);
	}
	else {
		player1->setTurnText(oppsTurn);
		player2->setTurnText(yourTurn);
	}
}