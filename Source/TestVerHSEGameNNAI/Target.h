// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class TESTVERHSEGAMENNAI_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
		USceneComponent *Root_Target;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *SM_Target;

	UPROPERTY(EditAnywhere)
		UBoxComponent *Box_Target;
	
	UFUNCTION()
		void OnEnemyEnterTargetBox(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
