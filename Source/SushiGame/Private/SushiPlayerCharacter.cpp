// Fill out your copyright notice in the Description page of Project Settings.


#include "SushiPlayerCharacter.h"

// Sets default values
ASushiPlayerCharacter::ASushiPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASushiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASushiPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASushiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

