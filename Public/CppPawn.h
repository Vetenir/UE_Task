#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CppPawn.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class CPP_PROJECT_API ACppPawn : public APawn
{
	GENERATED_BODY()

public:
	ACppPawn();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CollisionCylinder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* CharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;


protected:
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
};
