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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* TableOrderWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TableID;

	UPROPERTY(ReplicatedUsing = OnRep_OrderName)
	FName CurrentOrderName;

	UFUNCTION()
	void UpdateFloatingOrderText(const FName& OrderName);

	UFUNCTION()
	void OnRep_OrderName();

	UFUNCTION()
	void ClearFloatingOrderText();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
