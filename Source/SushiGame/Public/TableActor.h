#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TableActor.generated.h"

UCLASS()
class SUSHIGAME_API ATableActor : public AActor
{
	GENERATED_BODY()

public:	
	ATableActor();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TableMesh;

	// Optionally assign a visible ID to the table for UI/debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TableID = 0;
};