// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class TESTVERHSEGAMENNAI_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AEnemy> EnemyToSpawn;
	
	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnDelay;

private:
	void SpawnEnemy();
};
