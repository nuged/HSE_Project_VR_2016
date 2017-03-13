// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "Target.h"
#include "Enemy.h"


// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// set root
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root_Target"));
	
	// set static mesh
	SM_Target = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM_Target"));
	SM_Target->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// set collision box
	Box_Target = CreateDefaultSubobject<UBoxComponent>(FName("Box_Taget"));
	Box_Target->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	Box_Target->bGenerateOverlapEvents = true;
	Box_Target->OnComponentBeginOverlap.AddDynamic(this, &ATarget::OnEnemyEnterTargetBox);
	Box_Target->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATarget::OnEnemyEnterTargetBox(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AEnemy *Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
		Enemy->Destroy();
}

