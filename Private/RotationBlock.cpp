#include "RotationBlock.h"

ARotationBlock::ARotationBlock()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Asset(TEXT("/Game/Resources/Shapes/Grass_Block.Grass_Block"));
	if (SM_Asset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(SM_Asset.Object);
	}

	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 100.0f;
	RotationRange = 100.0f;
}

void ARotationBlock::BeginPlay()
{
	Super::BeginPlay();
}

void ARotationBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector VectorOffset = FVector(0.0f, RotationRange * DeltaTime, 0.0f);
	FRotator RotatorOffset = FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f);

	AddActorLocalOffset(VectorOffset);
	AddActorLocalRotation(RotatorOffset);
}

