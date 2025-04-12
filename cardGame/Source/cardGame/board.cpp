// Fill out your copyright notice in the Description page of Project Settings.


#include "board.h"

// Sets default values
Aboard::Aboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<UStaticMeshComponent>("root");
	plane = CreateDefaultSubobject<UStaticMeshComponent>("plane");
	cardBorder11 = CreateDefaultSubobject<UWidgetComponent>("cardBorder11");
	cardBorder12 = CreateDefaultSubobject<UWidgetComponent>("cardBorder12");
	cardBorder13 = CreateDefaultSubobject<UWidgetComponent>("cardBorder13");
	cardBorder14 = CreateDefaultSubobject<UWidgetComponent>("cardBorder14");
	cardBorder15 = CreateDefaultSubobject<UWidgetComponent>("cardBorder15");

	cardBorder21 = CreateDefaultSubobject<UWidgetComponent>("cardBorder21");
	cardBorder22 = CreateDefaultSubobject<UWidgetComponent>("cardBorder22");
	cardBorder23 = CreateDefaultSubobject<UWidgetComponent>("cardBorder23");
	cardBorder24 = CreateDefaultSubobject<UWidgetComponent>("cardBorder24");
	cardBorder25 = CreateDefaultSubobject<UWidgetComponent>("cardBorder25");

	RootComponent = root;

	plane->SetupAttachment(root);
	cardBorder11->SetupAttachment(root);
	cardBorder12->SetupAttachment(root);
	cardBorder13->SetupAttachment(root);
	cardBorder14->SetupAttachment(root);
	cardBorder15->SetupAttachment(root);

	cardBorder21->SetupAttachment(root);
	cardBorder22->SetupAttachment(root);
	cardBorder23->SetupAttachment(root);
	cardBorder24->SetupAttachment(root);
	cardBorder25->SetupAttachment(root);


}

// Called when the game starts or when spawned
void Aboard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

