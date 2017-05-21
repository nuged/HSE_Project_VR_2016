// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "EnemySpawner.h"
#include "Enemy.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnDelay = 2.0f;
	AbleToSpawn = false;
	FirstTimeDelay = 10.0f;
	EndDelay = 30.0f;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true, FirstTimeDelay);
}

void AEnemySpawner::SpawnEnemy()
{
	if (!AbleToSpawn) {
		AbleToSpawn = true;
		GetWorldTimerManager().SetTimer(EndTimer, this, &AEnemySpawner::ShowSuccess, EndDelay, false);
	}

	UWorld *const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		AEnemy *SpawnedEnemy = World->SpawnActor<AEnemy>(EnemyToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AEnemySpawner::ShowSuccess()
{
	if (wSuccess) {
		APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		MySuccess = CreateWidget<class UUserWidget>(PlayerController, wSuccess);
		if (MySuccess) {
			MySuccess->AddToViewport();
		}
		PlayerController->SetPause(true);
	}
}

