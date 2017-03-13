// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "EnemySpawner.h"
#include "Enemy.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnDelay = 2.0f;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);
}

// Called every frame
void AEnemySpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AEnemySpawner::SpawnEnemy()
{
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

