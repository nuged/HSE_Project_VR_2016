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

	// Success widget for successful game ending
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wSuccess;

	// Variable to hold the widget After Creating it.
	class UUserWidget *MySuccess;

	// Spawning indicator
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Spawning)
	bool AbleToSpawn;
	
	// Timer for finishing
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Timing)
	FTimerHandle EndTimer;

	// Seconds before finish
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Timing)
	float EndDelay;

	// Interval between spawnings
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Timing)
	float SpawnDelay;

	// first time delay beffore spawning
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Timing)
	float FirstTimeDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Spawning)
	TSubclassOf<class AEnemy> EnemyToSpawn;

private:
	FTimerHandle SpawnTimer;
	void SpawnEnemy();
	void ShowSuccess();
};
