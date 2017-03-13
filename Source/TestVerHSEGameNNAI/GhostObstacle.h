// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GhostObstacle.generated.h"

UCLASS()
class TESTVERHSEGAMENNAI_API AGhostObstacle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGhostObstacle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	// Visual and not only representation
	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;

	// object to replace with
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARealObstacle> RealObstacleToSpawn;
	
	//collision box 
	UPROPERTY(EditAnywhere)
		UBoxComponent *Box_GhostObstacle;
	
	// Replace with a real one
	void Replace();

	// delete an object
	void Delete();


	// drag
	void OnDragStarted();
	void OnDragFinished();

	// rotation
	void OnRotateStarted();
	void OnRotateFinished();

private:
	bool bLeftButtonIsPressed;
	bool bRightButtonIsPressed;
	// controller
	APlayerController *MyController;
};
