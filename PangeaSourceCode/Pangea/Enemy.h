// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PangeaCharacter.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PANGEA_API AEnemy : public APangeaCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UClass* _WeaponClass;
	AWeapon* _Weapon;
	
	APawn* _chasedTarget = nullptr;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Attack() override;

	void DieProcess() override;

	UFUNCTION(BlueprintCallable, Category = "Pangea|Enemy")
	void Chase(APawn* targetPawn);

};
