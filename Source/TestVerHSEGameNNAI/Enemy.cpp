// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "Enemy.h"
#include "NN_AIController.h"
#include "ANN.h"
#include "Target.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	step_distance = 20.0f;
	Norminator = 2000;
	proc = false;
	flag = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Search and set target
	AIC = Cast<ANN_AIController>(GetController());
	TActorIterator<ATarget> TargetIter(GetWorld());
	Target = *TargetIter;
	TargetLocation = Target->GetActorLocation();
	ToGo = GetActorLocation();
}

void AEnemy::Tick(float DeltaSeconds)
{
	if (!proc) {
		proc = true;
		FVector OwnLocation = GetActorLocation();
		FVector temp = OwnLocation - TargetLocation;
		float Distance = temp.Size() / Norminator;
		float X = OwnLocation.X / Norminator;
		float Y = OwnLocation.Y / Norminator;
		float Yaw = GetActorRotation().Yaw / Norminator;
		TArray<float> input;
		input = { Distance, 1 - Distance, X, 1 - X, Y, 1 - Y, Yaw, 1 - Yaw };
		int action = Target->NN->ChooseAction(input);
		Implement(action);
		flag = true;
	}

	if (flag && GetVelocity().IsZero()) {
		flag = false;
		FVector temp = ToGo - TargetLocation;
		float reward = 1 - temp.Size() / Norminator;
		Send(reward);
		proc = false;
	}
}

inline int AEnemy::Send(float reward)
{
	Target->NN->WModify(reward);
	return 0;
}

inline int AEnemy::Implement(int action)
{
	float angle = 360.0 / 8 * action;
	ToGo.X += cos(angle) * step_distance;
	ToGo.Y += sin(angle) * step_distance;
	AIC->MoveToLocation(ToGo);
	return 0;
}
