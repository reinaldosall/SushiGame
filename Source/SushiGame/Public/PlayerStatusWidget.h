#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.generated.h"

class UTextBlock;

UCLASS()
class SUSHIGAME_API UPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateDeliveryStatus(const FString& Text);

	UFUNCTION(BlueprintCallable)
	void UpdateScore(const FString& ScoreStr);

	UFUNCTION(BlueprintCallable)
	void UpdateStatus(const FString& RecipeName, const FString& StepDescription);

	UFUNCTION(BlueprintCallable)
	void ShowTemporaryMessage(const FString& Message, float Duration = 2.0f);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeliverStatusText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FeedbackText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ProgressText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RecipeNameText;
};