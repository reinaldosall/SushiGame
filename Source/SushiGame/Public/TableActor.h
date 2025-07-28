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

	// Componente raiz
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	// Mesh da mesa
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TableMesh;

	// Widget flutuante com o nome da ordem
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* TableOrderWidget;

	// Nome da receita esperada
	UPROPERTY(ReplicatedUsing = OnRep_OrderName)
	FName CurrentOrderName;

	UFUNCTION()
	void OnRep_OrderName();

	// Feedback visual de acerto/erro
	FTimerHandle FeedbackResetTimer;

public:	
	// Atualiza o nome da ordem
	void UpdateFloatingOrderText(const FName& OrderName);
	void ClearFloatingOrderText();

	void SetFeedbackText(const FString& Text);
	void ResetFeedbackText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order")
	int32 TableID = -1;

	UPROPERTY(ReplicatedUsing = OnRep_FeedbackVisualText)
	FString FeedbackVisualText;

	UFUNCTION()
	void OnRep_FeedbackVisualText();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};