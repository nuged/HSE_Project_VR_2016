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

	AutoPossessPlayer = EAutoReceiveInput::Disabled;
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

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Decal/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(32.0f, 64.0f, 64.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
}

// Called when the game starts or when spawned
void AGhostObstacle::BeginPlay()
{
	Super::BeginPlay();
	MyController = GetWorld()->GetFirstPlayerController();
	if (MyController)
		MyController->bAutoManageActiveCameraTarget = false;
}

// Called every frame
void AGhostObstacle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FHitResult TraceResult(ForceInit);
	if (MyController)
		MyController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, TraceResult);

	if (CursorToWorld) {
			FVector CursorFV = TraceResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
	}

	if (bLeftButtonIsPressed) {
		SetActorLocation(TraceResult.Location);
	}

	if (bRightButtonIsPressed) {
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
	CursorToWorld->ToggleVisibility(false);
}

void AGhostObstacle::OnRotateFinished()
{
	bRightButtonIsPressed = false;
	CursorToWorld->ToggleVisibility(true);
}
