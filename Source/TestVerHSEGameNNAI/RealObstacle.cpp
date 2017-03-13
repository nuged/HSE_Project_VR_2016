// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "RealObstacle.h"
#include "Enemy.h"


// Sets default values
ARealObstacle::ARealObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root_Obstacle = CreateDefaultSubobject<USceneComponent>(FName("Root_Obstacle"));
	RootComponent = Root_Obstacle;

	SM_Obstacle = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM_Obstacle"));
	SM_Obstacle->AttachToComponent(Root_Obstacle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Box_Obstacle = CreateDefaultSubobject<UBoxComponent>(FName("Box_Taget"));
	Box_Obstacle->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	Box_Obstacle->bGenerateOverlapEvents = true;
	Box_Obstacle->OnComponentBeginOverlap.AddDynamic(this, &ARealObstacle::OnEnemyEnterObstacleBox);
	Box_Obstacle->AttachToComponent(Root_Obstacle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void ARealObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARealObstacle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARealObstacle::OnEnemyEnterObstacleBox(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AEnemy *Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
		Enemy->Destroy();
}

