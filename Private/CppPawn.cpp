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
		// IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
		if (ACppPlayerController* PlayerController = Cast<ACppPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				// IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ACppPawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				// IA_Look 액션 마우스가 "움직일 때" Look() 호출
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

	// 현재 위치 가져오기
	FVector CurrentLocation = GetActorLocation();

	// 이동 방향 계산
	FVector MoveForward = GetActorForwardVector() * MoveInput.X * 10.0f; // 속도 조절 (10.0f 조정 가능)
	FVector MoveRight = GetActorRightVector() * MoveInput.Y * 10.0f;

	// 새 위치 설정
	SetActorLocation(CurrentLocation + MoveForward + MoveRight, true);
}

void ACppPawn::Look(const FInputActionValue& value)
{
	// 마우스의 X, Y 움직임을 2D 축으로 가져옴
	FVector2D LookInput = value.Get<FVector2D>();

	// 컨트롤러가 유효한지 확인
	if (Controller && Controller->IsLocalPlayerController())
	{
		// 현재 컨트롤러의 회전 값을 가져옴
		FRotator CurrentRotation = Controller->GetControlRotation();
		// 회전 속도 조절
		float RotationSpeed = 1.0f;

		// Yaw 입력 처리 (좌우 회전)
		float YawInput = LookInput.X * RotationSpeed;
		CurrentRotation.Yaw += YawInput;

		// Pitch 입력 처리 (상하 회전)
		float PitchInput = LookInput.Y * RotationSpeed;
		CurrentRotation.Pitch += PitchInput;

		// Pitch 회전을 특정 범위로 제한
		CurrentRotation.Pitch = FMath::ClampAngle(CurrentRotation.Pitch, -90.0f, 90.0f);

		// 컨트롤러의 회전 값을 업데이트
		Controller->SetControlRotation(CurrentRotation);
		SetActorRotation(CurrentRotation);
	}
}



