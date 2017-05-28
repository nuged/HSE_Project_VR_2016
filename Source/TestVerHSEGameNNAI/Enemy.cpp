// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "Enemy.h"
#include "NN_AIController.h"
#include "ANN.h"
#include "Target.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	step_distance = 20.0f;
	Norminator = 2300;
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
	TargetLocation.Z = 0;
	ToGo = GetActorLocation();
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!proc) {
		proc = true;
		FVector OwnLocation = GetActorLocation();
		OwnLocation.Z = 0;
		FVector temp = OwnLocation - TargetLocation;
		float Distance = temp.Size() / Norminator;
		temp = OwnLocation - FVector(0, 0, 0);
		float R = temp.Size() / Norminator;
		FRotator look = UKismetMathLibrary::FindLookAtRotation(FVector(0, 0, 0), OwnLocation);
		float phi = look.Yaw;
		if (phi < 0)
			phi = 360 + phi;
		float Yaw = GetActorRotation().Yaw;
		if (Yaw < 0)
			Yaw = 360 + Yaw;
		phi /= Norminator;
		Yaw /= Norminator;
		state = { Distance, R, phi, Yaw, 1 };
		TArray<int> result = Target->NN->ChooseAction(state);
		neuron = result[0];
		int action = result[1];
		Implement(action);
		flag = true;
	}

	if (flag && GetVelocity().IsZero()) {
		flag = false;
		FVector temp = ToGo - TargetLocation;
		float val = exp(-temp.Size() / Norminator);
		SetReward(val);
		Send();
		proc = false;
	}
}

inline int AEnemy::Send()
{
	Target->NN->AddToQueue(neuron, state);
	return 0;
}

inline int AEnemy::SetReward(float value)
{
	state[4] = value;
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
