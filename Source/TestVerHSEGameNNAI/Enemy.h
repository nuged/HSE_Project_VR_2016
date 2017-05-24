// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
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

	inline void Send(float reward);

	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree *BT_Enemy;

	UPROPERTY(EditAnywhere, Category = Movement)
		float step_distance;

	UPROPERTY(EditAnywhere, Category = Movement)
		float Norminator;

	UPROPERTY(EditAnywhere, Category = Movement)
		TSubclassOf<class APointToGo> Dest;

private:
	class ATarget *Target;
	class ANN_AIController *AIC;
	bool proc;
	bool flag;
	FVector TargetLocation;
	FVector ToGo;
	inline void Implement(int action);
};
