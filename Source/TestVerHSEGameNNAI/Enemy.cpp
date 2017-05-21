// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "Enemy.h"
#include "NN_AIController.h"
#include "Target.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	ANN_AIController *AIController = Cast<ANN_AIController>(GetController());
	TActorIterator<ATarget> TargetIter(GetWorld());
	ATarget *Actor = *TargetIter;

	if (AIController) {
		AIController->SetObjectToMoveTo(Actor);
	}
}
