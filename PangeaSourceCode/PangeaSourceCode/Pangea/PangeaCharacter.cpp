// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangeaCharacter.h"
#include "PangeaAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

APangeaCharacter::APangeaCharacter()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	
}

void APangeaCharacter::BeginPlay()
{
	_AnimInstance = Cast<UPangeaAnimInstance>(GetMesh()->GetAnimInstance());
	_HealthPoints = HealthPoints;
}

void APangeaCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void APangeaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int APangeaCharacter::GetHealthPoints()
{
	return _HealthPoints;
}

bool APangeaCharacter::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APangeaCharacter::CanAttack()
{
	return (_AttackCountingDown <= 0.0f && _AnimInstance->State == ECharacterState::Locomotion);
}

bool APangeaCharacter::IsAttacking()
{
	return (_AnimInstance->State == ECharacterState::Attack);
}

void APangeaCharacter::Attack()
{
	_AttackCountingDown = AttackInterval;
}

void APangeaCharacter::Hit(int damage)
{
	if(IsKilled())
	{
		return;
	}

	_HealthPoints -= damage;

	_AnimInstance->State = ECharacterState::Hit;

	if(IsKilled())
	{
		PrimaryActorTick.bCanEverTick = false;
	}
}

void APangeaCharacter::DieProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	Destroy();
	GEngine->ForceGarbageCollection(true);
}


