
#include "PlayerStatusWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h" 

void UPlayerStatusWidget::UpdateDeliveryStatus(const FString& Text)
{
	if (DeliverStatusText)
	{
		DeliverStatusText->SetText(FText::FromString(Text));
	}
}

void UPlayerStatusWidget::UpdateScore(const FString& ScoreStr)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(ScoreStr));
	}
}
 
void UPlayerStatusWidget::UpdateStatus(const FString& RecipeName, const FString& StepDescription)
{
	
	if (RecipeNameText)
	{
		RecipeNameText->SetText(FText::FromString(RecipeName));
	}

	if (ProgressText)
	{
		ProgressText->SetText(FText::FromString(StepDescription));
	}
	UE_LOG(LogTemp, Warning, TEXT("Widget::UpdateStatus — Recipe: %s, Step: %s"),
		*RecipeName, *StepDescription);
	UE_LOG(LogTemp, Warning, TEXT("[HUD] UpdateStatus called — Recipe: %s, Step: %s"), *RecipeName, *StepDescription);

}

void UPlayerStatusWidget::ShowTemporaryMessage(const FString& Message, float Duration)
{
	if (FeedbackText)
	{
		FeedbackText->SetText(FText::FromString(Message));
		FeedbackText->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (FeedbackText)
			{
				FeedbackText->SetVisibility(ESlateVisibility::Collapsed);
			}
		}, Duration, false);
	}
}