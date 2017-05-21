// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ObstacleBrush.generated.h"

UCLASS()
class TESTVERHSEGAMENNAI_API AObstacleBrush : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleBrush();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	/** Visual component */
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Visual")
		USceneComponent* OurVisibleComponent;

	/** A real obstacle that will replace out chost one */
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARealObstacle> RealObstacleToSpawn;

	/** Box shape to detect collisions */
	UPROPERTY(EditAnywhere, Category = "Collision")
		UBoxComponent *Box_Brush;

};
