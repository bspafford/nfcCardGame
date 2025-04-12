// Fill out your copyright notice in the Description page of Project Settings.


#include "card.h"
#include "Json.h"

// external classes
#include "cardActor.h"
#include "MyGameMode.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Fonts/SlateFontInfo.h"

// network stuff
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

THIRD_PARTY_INCLUDES_START
#include <winscard.h>
THIRD_PARTY_INCLUDES_END

#define MAX_ATR_SIZE 33
#define MAX_READERNAME 256

SCARDCONTEXT applicationContext;
LPSTR reader = NULL;
SCARDHANDLE connectionHandler;
DWORD activeProtocol;

// Sets default values
Acard::Acard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create components
	root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rootComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));

	// Attach components
	RootComponent = root;
	SpringArmComp->SetupAttachment(root);
	CameraComp->SetupAttachment(SpringArmComp);

	bReplicates = true;

}

// Called when the game starts or when spawned
void Acard::BeginPlay()
{
	Super::BeginPlay();

	// add widgets to viewport
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (controller && controller->GetPawn() == this) {
		controller->bShowMouseCursor = true;
		
		cardSelectionWidget = CreateWidget<UcardSelectionWidget>(controller, cardSelectionWidgetClass);
		if (cardSelectionWidget)
			cardSelectionWidget->AddToViewport();
		playerHealthWidget = CreateWidget<UplayerHealthWidget>(controller, playerHealthClass);
		if (playerHealthWidget) {
			playerHealthWidget->AddToViewport();
		}
	}

	playerJoined();
}

// Called every frame
void Acard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::B)) {
		UID = scanNFC();
	}

	if (!HasAuthority()) // make sure its not server
		UID = scanNFC();
}

void Acard::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAction("anyKey", EInputEvent::IE_Pressed, this, &Acard::KeyPressed);
}

void Acard::KeyPressed(FKey key) {
	int keyPressed = FCString::Atoi(*key.GetDisplayName().ToString());

	if (keyPressed >= 1 && keyPressed <= 5) {
		updateActiveSlot(keyPressed - 1);
		//SetPushReplicatedVariable(keyPressed);
	}
}

void Acard::updateActiveSlot_Implementation(int num) {
	activeCardSlot = num;
}

FString Acard::scanNFC() {

	if (!establishContext()) {
		return "";
	}
	if (!listReaders()) {
		return "";
	}
	if (!connectToCard()) {
		return "";
	}
	if (!getCardInformation()) {
		return "";
	}

	if (UID == "") {
		UID = sendUIDCommand();
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString::Printf(TEXT("UID: %s"), *UID));
		if (UID != "00:00:00:00:00:00" && UID != "" && canPlaceCard) {
			UE_LOG(LogTemp, Warning, TEXT("UID thingy"));
			getRequest(UID);
		}
	}

	disconnectFromCard();
	freeListReader();
	releaseContext();
	return UID;
}

bool Acard::establishContext() {
	LONG status = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &applicationContext);
	if (status == SCARD_S_SUCCESS) {
		return true;
	}
	else {
		return false;
	}
}
void Acard::releaseContext() {
	LONG status = SCardReleaseContext(applicationContext);
	if (status != SCARD_S_SUCCESS) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Release context error")));
		UE_LOG(LogTemp, Warning, TEXT("Establish context error"));
	}
}


bool Acard::listReaders() {
	DWORD readers = SCARD_AUTOALLOCATE;

	LONG status = SCardListReaders(applicationContext, NULL, (LPWSTR)&reader, &readers);
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString::Printf(TEXT("hello  : %s"), (LPWSTR)reader));
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString::Printf(TEXT("hellos: %s"), *FString::FormatAsNumber(readers)));

	if (status == SCARD_S_SUCCESS) {
		return true;
	}
	else {
		return false;
	}

	
}
void Acard::freeListReader() {
	LONG status = SCardFreeMemory(applicationContext, reader);
	if (status != SCARD_S_SUCCESS) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Free reader list error")));
		UE_LOG(LogTemp, Warning, TEXT("Establish context error"));
	}
}


bool Acard::connectToCard() {

	activeProtocol = -1;
	LONG status = SCardConnect(applicationContext, (LPCWSTR)reader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &connectionHandler, &activeProtocol);
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString::Printf(TEXT("hello: %s"), reader));
	if (status == SCARD_S_SUCCESS) {
		return true;
	}
	else {
		return false;
	}
}
void Acard::disconnectFromCard() {

	LONG status = SCardDisconnect(connectionHandler, SCARD_LEAVE_CARD);
	if (status != SCARD_S_SUCCESS) {
		UE_LOG(LogTemp, Warning, TEXT("Establish context error"));
	}
}


bool Acard::getCardInformation() {

	BYTE ATR[MAX_ATR_SIZE] = "";
	DWORD ATRLength = sizeof(ATR);
	char readerName[MAX_READERNAME] = "";
	DWORD readerLength = sizeof(readerName);
	DWORD readerState;
	DWORD readerProtocol;

	LONG status = SCardStatus(connectionHandler, (LPWSTR)readerName, &readerLength, &readerState, &readerProtocol, ATR, &ATRLength);
	if (status == SCARD_S_SUCCESS) {
		return true;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Get card information error")));
	}

	return false;
}

FString Acard::sendUIDCommand() {

	const SCARD_IO_REQUEST* pioSendPci = SCARD_PCI_T0;
	SCARD_IO_REQUEST pioRecvPci;
	uint8_t response[300];
	unsigned long responseLength = sizeof(response);

	switch (activeProtocol) {
	case SCARD_PROTOCOL_T0:
		pioSendPci = SCARD_PCI_T0;
		pioRecvPci.dwProtocol = SCARD_PROTOCOL_T0;
		pioRecvPci.cbPciLength = 100;
		break;
	case SCARD_PROTOCOL_T1:
		pioSendPci = SCARD_PCI_T1;
		pioRecvPci.dwProtocol = SCARD_PROTOCOL_T1;
		pioRecvPci.cbPciLength = 100;
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Protocol not found"));
	}

	BYTE command[] = { 0xFF, 0xCA, 0x00, 0x00, 0x04 };
	int commandLength = sizeof(command);

	FString uid = "";
	LONG status = SCardTransmit(connectionHandler, pioSendPci, command, commandLength, &pioRecvPci, response, &responseLength);
	if (status == SCARD_S_SUCCESS) {
		for (int i = 0; i < commandLength; i++) {
		}
		for (int i = 0; i < (int)responseLength; i++) {
			uid.Append(FString::Printf(TEXT("%02X:"), response[i]));
		}

		if (sizeof(uid) > 10)
			uid.RemoveFromEnd(":");
	}
	else {
		if (status != NULL) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Send command error: %d"), status));
			return "00:00:00:00:00:00";
		}
	}

	return uid;
}

void Acard::serverSpawnCard_Implementation(int cardSlot, const FString& cardUID, const FString& name, const FString& description, const int level, const int health, const int damage) {
	spawnCard(cardSlot, cardUID, name, description, level, health, damage);
	
}

void Acard::spawnCard_Implementation(int cardSlot, const FString &cardUID, const FString &name, const FString &description, const int level, const int health, const int damage) {
	
	if (HasAuthority()) // dont want server
		return;


	FVector location = GetActorLocation();
	FVector Offset;
	FRotator rotation = FRotator{ 0, 0, 0 };

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn() == this) // if not controlled pawn
		Offset = FVector{ 0, -140.f * cardSlot + 280, 0 };
	else
		Offset = FVector{ 0, 140.f * cardSlot + -280, 0 };

	if (location.X < 0) {
		rotation = FRotator{ 0, 180, 0 };
		Offset *= -1;
	}

	location += Offset;
	
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn() && turn) {
		if (cardsOnTable[cardSlot] == NULL) {
			AActor* spawnedCard = GetWorld()->SpawnActor<AActor>(cardActor, location, rotation);
			// add card to list
			cardsOnTable[cardSlot] = spawnedCard;

			AcardActor* cardScript = Cast<AcardActor>(spawnedCard);

			if (cardScript)
				cardScript->setupCard(UID, name, health, damage);

			// once finished spawning check to see if you can battle
			checkBattle();
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0, FColor::Red, TEXT("spot is taken, choose another"));
	}
}

void Acard::isTableFull_Implementation(int p) {
	bool tableFull = true;
	
	for (int i = 0; i < sizeof(cardsOnTable) / sizeof(cardsOnTable[0]); i++) {
		if (!cardsOnTable[i]) {
			tableFull = false;
			break;
		}
	}

	if (tableFull) {
		setTableFull(p);
	}
}

void Acard::setTableFull_Implementation(int p) {
	AMyGameMode* gameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode) {
		if (p == 1)
			gameMode->player1Went = true;
		else
			gameMode->player2Went = true;

		gameMode->checkBattle(this);
	}
}

void Acard::getRequest(FString uid) {

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &Acard::OnResponseRecieved);
	Request->SetURL("http://10.0.0.184/searchUID.php?UID=" + uid);
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void Acard::OnResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) {

	if (HasAuthority())
		return;

	// could be the error text
	if (Response && Response->GetContentAsString() == "Error") {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("response parse error"));
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	FcardInfo cardInfo;
	cardInfo.UID = *ResponseObj->GetStringField("UID");
	cardInfo.name = *ResponseObj->GetStringField("name");
	cardInfo.description = *ResponseObj->GetStringField("description");
	cardInfo.level = FCString::Atoi(*ResponseObj->GetStringField("level"));
	cardInfo.health = FCString::Atoi(*ResponseObj->GetStringField("health"));
	cardInfo.damage = FCString::Atoi(*ResponseObj->GetStringField("damage"));

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn() && turn) {
		serverSpawnCard(activeCardSlot, cardInfo.UID, cardInfo.name, cardInfo.description, cardInfo.level, cardInfo.health, cardInfo.damage);
	}
}

void Acard::playerJoined_Implementation() {
	AMyGameMode* gameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode) {
		gameMode->playerJoined();
	}
}

void Acard::checkBattle_Implementation() {
	AMyGameMode* gameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode && UGameplayStatics::GetPlayerController(GetWorld(), gameMode->currentPlayersTurn)->GetPawn() == this && turn) {
		gameMode->checkBattle(this);
	}
}

void Acard::setTurnText_Implementation(const FText& text) {

	if (HasAuthority() || !cardSelectionWidget)
		return;

	cardSelectionWidget->turnText->SetText(text);

	
}

void Acard::takeDamage_Implementation(Acard* other) {
	otherCardRef = other;
	for (int i = 0; i < sizeof(cardsOnTable) / sizeof(cardsOnTable[0]); i++) {
		if (other->cardsOnTable[i]) {
			AcardActor* otherCard = Cast<AcardActor>(other->cardsOnTable[i]);
			if (cardsOnTable[i]) { // if theres another card
				// extra damage past card health does damage to player
				AcardActor* card = Cast<AcardActor>(cardsOnTable[i]);
				card->hp -= otherCard->damage;

				if (card->hp <= 0) {
					if (card->hp < 0)
						playerHealth += card->hp;
				} else
					card->setupCard(card->UID, card->name, card->hp, card->damage);
			}
			else { // take damage to players health
				playerHealth -= otherCard->damage;
			}
		}
	}

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, [this] {
		for (int i = 0; i < sizeof(cardsOnTable) / sizeof(cardsOnTable[0]); i++) {
			if (cardsOnTable[i]) {
				AcardActor* card = Cast<AcardActor>(cardsOnTable[i]);
				if (card->hp < 0) {
					card->Destroy();
					cardsOnTable[i] = NULL;
				}
			}
		}
		updatePlayerHealthWidget(playerHealth, otherCardRef->playerHealth);
		}, .01f, false);
}

void Acard::updatePlayerHealthWidget_Implementation(int p1Health, int p2Health) {
	if (!playerHealthWidget)
		return;

	playerHealthWidget->player1Health->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), p1Health)));
	playerHealthWidget->player2Health->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), p2Health)));
}

void Acard::updateLoc_Implementation(FVector alpha, int indexNum) {
	if (HasAuthority())
		return;

	//for (int i = 0; i < sizeof(cardsOnTable) / sizeof(cardsOnTable[0]); i++) {
		if (cardsOnTable[indexNum]) {
			AcardActor* currCard = Cast<AcardActor>(cardsOnTable[indexNum]);
			FVector loc = currCard->GetActorLocation();
			
			FVector a = alpha;
			if (loc.X < 0)
				a.X *= -1;

			loc = a + GetActorLocation() + FVector{ 0, loc.Y, 0 };

			currCard->SetActorLocation(loc);
		}
	//}
}

void Acard::test_Implementation() {
	serverTest(cardsOnTable[0] != NULL, cardsOnTable[1] != NULL, cardsOnTable[2] != NULL, cardsOnTable[3] != NULL, cardsOnTable[4] != NULL);
}

void Acard::serverTest_Implementation(bool slot1, bool slot2, bool slot3, bool slot4, bool slot5) {

	bool validSlots[5];

	validSlots[0] = slot1;
	validSlots[1] = slot2;
	validSlots[2] = slot3;
	validSlots[3] = slot4;
	validSlots[4] = slot5;

	AMyGameMode* gameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
		gameMode->shouldAttackSlot(validSlots[0], validSlots[1], validSlots[2], validSlots[3], validSlots[4]);
}


// network stuff
void Acard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(Acard, turn);
	DOREPLIFETIME(Acard, activeCardSlot);
	DOREPLIFETIME(Acard, canPlaceCard);
	DOREPLIFETIME(Acard, cardsOnTable);
	DOREPLIFETIME(Acard, playerHealth);
}