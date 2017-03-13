// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "NN_AIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTVERHSEGAMENNAI_API ANN_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANN_AIController();

	virtual void Possess(APawn *Pawn) override;
	
	void SetObjectToMoveTo(AActor *Actor);
	
protected:
	UBehaviorTreeComponent *BT_Comp;
	UBlackboardComponent *BB_Comp;
};
