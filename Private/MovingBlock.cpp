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

	// ���� ��ġ ��������
	FVector CurrentLocation = GetActorLocation();

	// ��ġ�� ������ ����� �ӵ� ����
	if (CurrentLocation.Y >= MoveRange || CurrentLocation.Y <= -MoveRange)
	{
		MoveSpeed *= -1; // ��ȣ ����
	}

	// �̵��� ������ ���
	FVector Offset = FVector(0.0f, MoveSpeed * DeltaTime, 0.0f);

	// Actor �̵�
	AddActorLocalOffset(Offset);
}

