// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PangeaCharacter.generated.h"

UCLASS(Blueprintable)
class APangeaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APangeaCharacter();

	UPROPERTY(EditAnywhere, Category = "Pangea Character Params")
	int HealthPoints = 100;

	UPROPERTY(EditAnywhere, Category = "Pangea Character Params")
	float Strength = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Pangea Character Params")
	float Armor = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Pangea Character Params")
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Pangea Character Params")
	float AttackInterval = 3.0f;

public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Pangea|Character", meta = (DisplayName = "Get HP"))
	int GetHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "Pangea|Character")
	bool IsKilled();
	
	UFUNCTION(BlueprintCallable, Category = "Pangea|Character")
	bool CanAttack();
	bool IsAttacking();

	virtual void Attack();
	virtual void Hit(int damage);
	virtual void DieProcess();
	
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UPangeaAnimInstance* _AnimInstance;
	int _HealthPoints;
	float _AttackCountingDown;
};

