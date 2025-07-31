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

	// Root scene component
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	// Static mesh representing the table
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TableMesh;

	// Widget component showing the current order
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* TableOrderWidget;

	// Current order name expected at this table
	UPROPERTY(ReplicatedUsing = OnRep_OrderName)
	FName CurrentOrderName;

	UFUNCTION()
	void OnRep_OrderName();

	// Timer to reset feedback text after a short delay
	FTimerHandle FeedbackResetTimer;

public:
	// Updates the floating text with the order name
	void UpdateFloatingOrderText(const FName& OrderName);

	// Clears the order name from the widget
	void ClearFloatingOrderText();

	// Shows a short feedback message ("CORRECT", "X", etc.)
	void SetFeedbackText(const FString& Text);

	// Resets the feedback text after delay
	void ResetFeedbackText();

	// Table ID used to identify it in the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order")
	int32 TableID = -1;

	// Feedback text shown temporarily over the table
	UPROPERTY(ReplicatedUsing = OnRep_FeedbackVisualText)
	FString FeedbackVisualText;

	UFUNCTION()
	void OnRep_FeedbackVisualText();

	// Ensures all clients refresh the widget text
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RefreshOrderName();

	void Multicast_RefreshOrderName_Implementation();

	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};