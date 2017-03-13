// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RealObstacle.generated.h"

UCLASS()
class TESTVERHSEGAMENNAI_API ARealObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealObstacle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
		USceneComponent *Root_Obstacle;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *SM_Obstacle;

	UPROPERTY(EditAnywhere)
		UBoxComponent *Box_Obstacle;

	UFUNCTION()
		void OnEnemyEnterObstacleBox(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
