#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingBlock.generated.h"

UCLASS()
class CPP_PROJECT_API AMovingBlock : public AActor
{
	GENERATED_BODY()
	
public:
	AMovingBlock();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Block|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|MoveSpeed")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|MoveRange")
	float MoveRange;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
