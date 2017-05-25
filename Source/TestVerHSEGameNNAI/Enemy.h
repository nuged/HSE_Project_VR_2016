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
	
	UFUNCTION(BlueprintCallable, Category = SettingNN)
	inline int Implement(int action);

	UFUNCTION(BlueprintCallable, Category=SettingNN)
	inline int Send(float reward);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
	class UBehaviorTree *BT_Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float step_distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Norminator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	TSubclassOf<class APointToGo> Dest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Target)
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	FVector ToGo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool proc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool flag;

private:
	class ATarget *Target;
	class ANN_AIController *AIC;
	
};
