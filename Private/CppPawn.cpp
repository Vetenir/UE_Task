#include "CppPawn.h"
#include "CppPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

ACppPawn::ACppPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionCylinder = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCylinder"));
	SetRootComponent(CollisionCylinder);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(CollisionCylinder);
	CollisionCylinder->InitCapsuleSize(37.0f, 93.0f);
	// CollisionCylinder->SetSimulatePhysics(false);

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(CollisionCylinder);
	// CharacterMesh->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CollisionCylinder);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

void ACppPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA�� �������� ���� ���� ���� ���� Controller�� ASpartaPlayerController�� ĳ����
		if (ACppPlayerController* PlayerController = Cast<ACppPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				// IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ACppPawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				// IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ACppPawn::Look
				);
			}
		}
	}
}

void ACppPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	// ���� ��ġ ��������
	FVector CurrentLocation = GetActorLocation();

	// �̵� ���� ���
	FVector MoveForward = GetActorForwardVector() * MoveInput.X * 10.0f; // �ӵ� ���� (10.0f ���� ����)
	FVector MoveRight = GetActorRightVector() * MoveInput.Y * 10.0f;

	// �� ��ġ ����
	SetActorLocation(CurrentLocation + MoveForward + MoveRight, true);
}

void ACppPawn::Look(const FInputActionValue& value)
{
	// ���콺�� X, Y �������� 2D ������ ������
	FVector2D LookInput = value.Get<FVector2D>();

	// ��Ʈ�ѷ��� ��ȿ���� Ȯ��
	if (Controller && Controller->IsLocalPlayerController())
	{
		// ���� ��Ʈ�ѷ��� ȸ�� ���� ������
		FRotator CurrentRotation = Controller->GetControlRotation();
		// ȸ�� �ӵ� ����
		float RotationSpeed = 1.0f;

		// Yaw �Է� ó�� (�¿� ȸ��)
		float YawInput = LookInput.X * RotationSpeed;
		CurrentRotation.Yaw += YawInput;

		// Pitch �Է� ó�� (���� ȸ��)
		float PitchInput = LookInput.Y * RotationSpeed;
		CurrentRotation.Pitch += PitchInput;

		// Pitch ȸ���� Ư�� ������ ����
		CurrentRotation.Pitch = FMath::ClampAngle(CurrentRotation.Pitch, -90.0f, 90.0f);

		// ��Ʈ�ѷ��� ȸ�� ���� ������Ʈ
		Controller->SetControlRotation(CurrentRotation);
		SetActorRotation(CurrentRotation);
	}
}



