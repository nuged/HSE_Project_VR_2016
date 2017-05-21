// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "ObstacleBrush.h"


// Sets default values
AObstacleBrush::AObstacleBrush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create and attach a visible object
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	OurVisibleComponent->SetupAttachment(RootComponent);

	// Collision Box
	Box_Brush = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	Box_Brush->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	Box_Brush->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AObstacleBrush::BeginPlay()
{
	Super::BeginPlay();
	
}
