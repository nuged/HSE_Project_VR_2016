// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "NN_AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ANN_AIController::ANN_AIController()
{
	NN = NewObject<UANN>();
	BT_Comp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BT_Comp"));
	BB_Comp = CreateDefaultSubobject<UBlackboardComponent>(FName("BB_Comp"));
}

void ANN_AIController::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);
	AEnemy *AIEnemy = Cast<AEnemy>(Pawn);
	if (AIEnemy) {
		if (AIEnemy->BT_Enemy->BlackboardAsset) {
			BB_Comp->InitializeBlackboard(*(AIEnemy->BT_Enemy->BlackboardAsset));
			BT_Comp->StartTree(*AIEnemy->BT_Enemy);
		}
	}
}

void ANN_AIController::SetObjectToMoveTo(AActor * Actor)
{
	if (BB_Comp) {
		BB_Comp->SetValueAsObject(FName("ObjectToMoveTo"), Actor);
	}
}
