// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ANN.h"
#include "Enemy.generated.h"

UCLASS()
class TESTVERHSEGAMENNAI_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
	class UBehaviorTree *BT_Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float step_distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Target)
	FVector TargetLocation;

	inline void Send(float new_reward);

	unsigned dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float norm;
	void Die();

protected:
	inline float CountReward();
	void MakeStep(unsigned direction);
	void GetAction();


private:
	class ATarget *Target;
	class ANN_AIController *AIC;
	float reward;
	bool flag;
	bool f;
	bool exec;
	TVector state;
	FVector ToGo;
};
