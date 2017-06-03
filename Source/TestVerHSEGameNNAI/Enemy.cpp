// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "Enemy.h"
#include "NN_AIController.h"
#include "Target.h"
#include <algorithm>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	step_distance = 150.0f;
	flag = true;
	f = false;
	exec = false;
	state.resize(2);
	norm = 18;
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
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (flag && GetVelocity().IsZero()) {
		flag = false;
		exec = true;
		GetAction();
	} else if (!GetVelocity().IsZero()) {
		f = true;
		exec = false;
	}
	if (f && GetVelocity().IsZero()) {
		f = false;
		Send(CountReward());
		flag = true;
	}
}

inline void AEnemy::Send(float new_reward)
{	
	exec = true;
	TVector target(8);
	if (new_reward >= reward) {
		std::fill(target.begin(), target.end(), -1);
		target[dir] = 1;
	}
	else {
		std::fill(target.begin(), target.end(), 1);
		target[dir] = -1;
	}

	Target->NN->AddToQueue(state, target);
	exec = false;
}

inline float AEnemy::CountReward()
{
	FVector temp = GetActorLocation();
	temp -= TargetLocation;
	float result = temp.Size();
	result = 1 / (1 + result);
	return result;
}

void AEnemy::MakeStep(unsigned direction)
{
	ToGo = GetActorLocation();
	float angle = direction * 2 * PI / 8;
	float x = step_distance * cos(angle);
	float y = step_distance * sin(angle);
	ToGo.X += x;
	ToGo.Y += y;
	AIC->MoveToLocation(ToGo);
}

void AEnemy::GetAction()
{
	FVector temp = GetActorLocation();
	FVector curr_location = temp;
	reward = CountReward();
	temp.X = trunc(temp.X / step_distance);
	temp.Y = trunc(temp.Y / step_distance);
	state[0] = temp.X / norm;
	state[1] = temp.Y / norm;
	dir = Target->NN->ChooseAction(state);
	MakeStep(dir);
}

void AEnemy::Die()
{
	while (exec);
	Destroy();
}
