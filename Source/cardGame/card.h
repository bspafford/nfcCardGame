// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Http.h"
#include "cardSelectionWidget.h"
#include "playerHealthWidget.h"
#include "Components/TextBlock.h"
#include "card.generated.h"

struct FcardInfo {
	FString UID = "";
	FString name = "";
	FString description = "";
	int level = 0;
	int health = 0;
	int damage = 0;
	int player = 0;
};

UCLASS()
class CARDGAME_API Acard : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	Acard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void KeyPressed(FKey key);

	FString scanNFC();

	bool establishContext();
	bool listReaders();
	bool connectToCard();

	bool getCardInformation();


	void disconnectFromCard();
	void freeListReader();
	void releaseContext();

	FString sendUIDCommand();

	void getRequest(FString UID);

	void OnResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//void recievedResponse(const FcardInfo& cardInfo);// const FcardInfo& UID);
	UFUNCTION(Server, Reliable)
	void playerJoined();
	void playerJoined_Implementation();

	UFUNCTION(Server, Reliable)
	void checkBattle();
	void checkBattle_Implementation();

public:
	UFUNCTION(Client, Reliable)
	void setTurnText(const FText& text);
	void setTurnText_Implementation(const FText& text);
	//void setTurnText_Implementation();

private:
	UFUNCTION(Server, Reliable)
	void serverSpawnCard(int cardSlot, const FString& cardUID, const FString& name, const FString& description, const int level, const int health, const int damage);
	void serverSpawnCard_Implementation(int cardSlot, const FString& cardUID, const FString& name, const FString& description, const int level, const int health, const int damage);

	UFUNCTION(NetMulticast, Reliable)
	void spawnCard(int cardSlot, const FString &cardUID, const FString &name, const FString &description, const int level, const int health, const int damage);
	void spawnCard_Implementation(int cardSlot, const FString &cardUID, const FString &name, const FString &description, const int level, const int health, const int damage);

	UPROPERTY(EditAnywhere)
	FString UID;

	// components
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* root;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;
public:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> cardActor;
	
	// widgets
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> cardSelectionWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> playerHealthClass;
	UPROPERTY(EditAnywhere)
	UplayerHealthWidget* playerHealthWidget;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UcardSelectionWidget* cardSelectionWidget;


	// player stats
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool turn = false;
	UPROPERTY(Replicated, EditAnywhere)
	bool canPlaceCard; // might not be secure to have on client
	UPROPERTY(Replicated, EditAnywhere)
	int playerHealth = 1000;
private:
	UPROPERTY(Replicated, EditAnywhere)
	int activeCardSlot = 0; // 0 cuz index
	UPROPERTY(Replicated, EditAnywhere)
	AActor* cardsOnTable[5];
	UPROPERTY(EditAnywhere)
	int playerMaxHealth;
	UPROPERTY()
	Acard* otherCardRef;

public:
	// player stat functions
	UFUNCTION(Server, Reliable)
	void updateActiveSlot(int slotNum);
	void updateActiveSlot_Implementation(int slotNum);
	UFUNCTION(Client, Reliable)
	void updatePlayerHealthWidget(int p1Health, int p2Health);
	void updatePlayerHealthWidget_Implementation(int p1Health, int p2Health);
	UFUNCTION(NetMulticast, Reliable)
	void updateLoc(FVector alpha, int indexNum);
	void updateLoc_Implementation(FVector alpha, int indexNum);
	UFUNCTION(NetMulticast, Reliable)
	void takeDamage(Acard* otherPlayer);
	void takeDamage_Implementation(Acard* otherPlayer);
	UFUNCTION(Client, Reliable)
	void isTableFull(int p);
	void isTableFull_Implementation(int p);
	UFUNCTION(Server, Reliable)
	void setTableFull(int p);
	void setTableFull_Implementation(int p);
	UFUNCTION(Client, Reliable)
	void test();
	void test_Implementation();
	UFUNCTION(Server, Reliable)
	void serverTest(bool slot1, bool slot2, bool slot3, bool slot4, bool slot5);
	void serverTest_Implementation(bool slot1, bool slot2, bool slot3, bool slot4, bool slot5);

	// networking
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
