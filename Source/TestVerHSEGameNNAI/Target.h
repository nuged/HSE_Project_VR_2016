// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ANN.h"
#include "Target.generated.h"

UCLASS()
class TESTVERHSEGAMENNAI_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();

	virtual void BeginPlay() override;

	// total HP of the target
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Health)
	int HP_Target;

	// Damage taken by the Target by every collision
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	int Damage;

	UPROPERTY(EditAnywhere, Category=Geometry)
	USceneComponent *Root_Target;

	UPROPERTY(EditAnywhere, Category=Geometry)
	UStaticMeshComponent *SM_Target;

	UPROPERTY(EditAnywhere, Category=Geometry)
	UBoxComponent *Box_Target;
	
	UANN *NN;

	UFUNCTION()
	void OnEnemyEnterTargetBox(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
		 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
