#include "MovingBlock.h"

AMovingBlock::AMovingBlock()
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
	MoveSpeed = 100.0f;
	MoveRange = 500.0f;
}

void AMovingBlock::BeginPlay()
{
	Super::BeginPlay();
}

void AMovingBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 위치 가져오기
	FVector CurrentLocation = GetActorLocation();

	// 위치가 범위를 벗어나면 속도 반전
	if (CurrentLocation.Y >= MoveRange || CurrentLocation.Y <= -MoveRange)
	{
		MoveSpeed *= -1; // 부호 반전
	}

	// 이동할 오프셋 계산
	FVector Offset = FVector(0.0f, MoveSpeed * DeltaTime, 0.0f);

	// Actor 이동
	AddActorLocalOffset(Offset);
}

