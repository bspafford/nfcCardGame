// Fill out your copyright notice in the Description page of Project Settings.


#include "cardActor.h"

#include "cardWidget.h"
#include "cardStruct.h"
#include "hologramWidget.h"
#include "hologramStruct.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"


// Sets default values
AcardActor::AcardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create Component
	root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	cardWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("cardWidget"));
	hologramWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("hologramWidget"));

	// attach component
	RootComponent = root;
	cardWidget->SetupAttachment(RootComponent);
	hologramWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AcardActor::BeginPlay()
{
	Super::BeginPlay();

	// get controled pawn
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (controller && controller->GetPawn())
		controlledPawn = Cast<Acard>(controller->GetPawn());

	FOnTimelineFloat progressUpdate;
	progressUpdate.BindUFunction(this, FName("timelineUpdate"));
	timeline.AddInterpFloat(curve, progressUpdate);
	timeline.SetLooping(true);
	timeline.PlayFromStart();
}

// Called every frame
void AcardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timeline.TickTimeline(DeltaTime);

	// get the location of the camera from the controlled pawn
	if (controlledPawn) {
		camPos = controlledPawn->CameraComp->GetComponentLocation();
		//camPos.Y /= 5;
		
		FVector Forward = camPos - hologramWidget->GetComponentLocation();
		FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector{0, 0, 1});
		Rot.Pitch /= 5.f;
		hologramWidget->SetWorldRotation(Rot);
		
		//hologramWidget->SetWorldRotation(camPos.Rotation());
	}
		
}

void AcardActor::setupCard(FString cardUID, FString cardName, float cardHp, float cardDamage) {
	UID = cardUID;
	name = cardName;
	hp = cardHp;
	damage = cardDamage;

	UcardWidget* cardWidgetScript = Cast<UcardWidget>(cardWidget->GetWidget());
	UhologramWidget* hologramWidgetScript = Cast<UhologramWidget>(hologramWidget->GetWidget());

	FcardStruct* cardDataRow = cardDatatable->FindRow<FcardStruct>(FName(cardName), "");
	FhologramStruct* hologramDataRow = hologramDatatable->FindRow<FhologramStruct>(FName(cardName), "");

	if (cardWidgetScript && cardDataRow && hologramWidgetScript && hologramDataRow) {
		// set cardWidget
		cardWidgetScript->thumbnail->SetBrushFromTexture(cardDataRow->thumbnail);
		cardWidgetScript->name->SetText(FText::FromString(name));

		// setu bonus stats
		if (bonusHp == 0) {
			cardWidgetScript->health->SetText(FText::FromString("HP: " + FString::FromInt(cardHp)));
			hologramWidgetScript->hologramHealth->SetText(FText::FromString("HP: " + FString::FromInt(cardHp)));
		}
		else {
			cardWidgetScript->health->SetText(FText::FromString("HP: " + FString::FromInt(cardHp) + "+" + FString::FromInt(bonusHp)));
			hologramWidgetScript->hologramHealth->SetText(FText::FromString("HP: " + FString::FromInt(cardHp) + "+" + FString::FromInt(bonusHp)));
		}

		if (bonusDamage == 0) {
			cardWidgetScript->damage->SetText(FText::FromString("ATK: " + FString::FromInt(cardDamage)));
			hologramWidgetScript->hologramDamage->SetText(FText::FromString("ATK: " + FString::FromInt(cardDamage)));
		}
		else {
			cardWidgetScript->damage->SetText(FText::FromString("ATK: " + FString::FromInt(cardDamage) + "+" + FString::FromInt(bonusDamage)));
			hologramWidgetScript->hologramDamage->SetText(FText::FromString("ATK: " + FString::FromInt(cardDamage) + "+" + FString::FromInt(bonusDamage)));
		}


		// set hologram
		hologramWidgetScript->hologramImage->SetBrushFromTexture(hologramDataRow->thumbnail);
	}
}

void AcardActor::timelineUpdate(float alpha) {
	// 75.0
	hologramWidget->SetRelativeLocation(FVector{ -37.50, 0, (alpha * 5.f) + 75.f });
}
