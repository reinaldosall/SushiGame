#include "SushiPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.h"

void ASushiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT(">>> SushiPlayerController::BeginPlay HIT <<<"));

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> IsLocalController is TRUE <<<"));

		if (PlayerStatusWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> PlayerStatusWidgetClass is valid: %s"), *PlayerStatusWidgetClass->GetName());

			PlayerStatusWidgetInstance = CreateWidget<UPlayerStatusWidget>(this, PlayerStatusWidgetClass);
			if (PlayerStatusWidgetInstance)
			{
				PlayerStatusWidgetInstance->AddToViewport(10);
				UE_LOG(LogTemp, Warning, TEXT(">>> PlayerStatusWidget successfully added to viewport! <<<"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT(">>> PlayerStatusWidgetInstance is NULL after CreateWidget <<<"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(">>> PlayerStatusWidgetClass is NULL <<<"));
		}
	}
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}
ASushiPlayerController::ASushiPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPlayerStatusWidget> WidgetClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_PlayerStatus"));
	if (WidgetClass.Succeeded())
	{
		PlayerStatusWidgetClass = WidgetClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Set PlayerStatusWidgetClass in constructor"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find WBP_PlayerStatus in constructor"));
	}
}