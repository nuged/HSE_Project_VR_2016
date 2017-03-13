// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "GhostObstacleMovementComp.generated.h"

/**
 * 
 */
UCLASS()
class TESTVERHSEGAMENNAI_API UGhostObstacleMovementComp : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	
};
