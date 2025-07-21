#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TableActor.generated.h"

class UWidgetComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* TableOrderWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TableID = 0;

	// Shows floating text
	void UpdateFloatingOrderText(const FName& OrderName);
	
	UFUNCTION(BlueprintCallable)
	void ClearFloatingOrderText();
};
