#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "CookwareActor.generated.h"

class AIngredientActor;

UCLASS()
class SUSHIGAME_API ACookwareActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACookwareActor();

	virtual void Tick(float DeltaTime) override;

	// Interação com ingrediente
	void OnInteract(AIngredientActor* Ingredient);

protected:
	virtual void BeginPlay() override;

	// Estado atual da cookware
	UPROPERTY()
	bool bIsCooking = false;

	UPROPERTY()
	bool bIsDone = false;

	// Jogador que iniciou o processo de cooking
	UPROPERTY()
	APlayerController* LockedPlayer = nullptr;

	// Timer do processo de cooking
	FTimerHandle CookingTimerHandle;

	// Callback após o tempo de cooking terminar
	UFUNCTION()
	void OnCookingFinished();
	
	// Simula tempo de preparo
	UPROPERTY(EditDefaultsOnly, Category = "Cooking")
	float ProcessingTime = 2.5f;

	// Ingrediente atual sendo processado
	UPROPERTY(ReplicatedUsing = OnRep_CurrentIngredient)
	AIngredientActor* CurrentIngredient;

	// Indica se está processando
	UPROPERTY(Replicated)
	bool bIsProcessing;

	// Widget de progresso
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* ProgressWidget;

	// Atualiza o widget com o estado do ingrediente
	void UpdateProgressWidget();

	// Chamado quando CurrentIngredient é replicado
	UFUNCTION()
	void OnRep_CurrentIngredient();

	// Conclui o processamento
	UFUNCTION()
	void OnProcessingComplete();

	// Inicia o processamento
	void StartProcessing(AIngredientActor* Ingredient);

	// Timer do processamento
	FTimerHandle ProcessingTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* CookwareMesh;

	// Replicação
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};