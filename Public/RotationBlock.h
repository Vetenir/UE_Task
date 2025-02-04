#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotationBlock.generated.h"

UCLASS()
class CPP_PROJECT_API ARotationBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ARotationBlock();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Block|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|RotationSpeed")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|RotationRange")
	float RotationRange;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
