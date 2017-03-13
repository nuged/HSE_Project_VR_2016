// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "RealObstacle.h"
#include "GhostObstacle.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AGhostObstacle::AGhostObstacle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bLeftButtonIsPressed = false;
	bRightButtonIsPressed = false;

	// Create a dummy root component we can attach things to.
	 RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	 // Create and attach a visible object
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	OurVisibleComponent->SetupAttachment(RootComponent);

	// Collision Box
	Box_GhostObstacle = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	Box_GhostObstacle->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	Box_GhostObstacle->SetupAttachment(RootComponent);

	/*
	// Create and attach a camera, set its offset and rotation
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-500.0f, 0.0f, 500.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	*/
}

// Called when the game starts or when spawned
void AGhostObstacle::BeginPlay()
{
	Super::BeginPlay();
	MyController = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void AGhostObstacle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bLeftButtonIsPressed) {
		FHitResult TraceResult(ForceInit);
		MyController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, TraceResult);
		SetActorLocation(TraceResult.Location);
	}
	if (bRightButtonIsPressed) {
		FHitResult TraceResult(ForceInit);
		MyController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, TraceResult);
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TraceResult.Location);
		SetActorRotation(NewRotation);
	}
}

// Called to bind functionality to input
void AGhostObstacle::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Drag", IE_Pressed, this, &AGhostObstacle::OnDragStarted);
	InputComponent->BindAction("Drag", IE_Released, this, &AGhostObstacle::OnDragFinished);
	InputComponent->BindAction("Rotate", IE_Pressed, this, &AGhostObstacle::OnRotateStarted);
	InputComponent->BindAction("Rotate", IE_Released, this, &AGhostObstacle::OnRotateFinished);
	InputComponent->BindAction("Delete", IE_Pressed, this, &AGhostObstacle::Delete);
	InputComponent->BindAction("SetUp", IE_Pressed, this, &AGhostObstacle::Replace);
}

void AGhostObstacle::Replace()
{
	UWorld *const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		ARealObstacle *RealObstacle = World->SpawnActor<ARealObstacle>(RealObstacleToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		if (RealObstacle)
			Destroy();
	}
}

void AGhostObstacle::Delete()
{
	Destroy();
}

void AGhostObstacle::OnDragStarted()
{
	bLeftButtonIsPressed = true;
}

void AGhostObstacle::OnDragFinished()
{
	bLeftButtonIsPressed = false;
}

void AGhostObstacle::OnRotateStarted()
{
	bRightButtonIsPressed = true;
}

void AGhostObstacle::OnRotateFinished()
{
	bRightButtonIsPressed = false;
}
