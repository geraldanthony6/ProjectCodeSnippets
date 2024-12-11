// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PangeaCharacter.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerAvatar.generated.h"

UCLASS(Blueprintable)
class PANGEA_API APlayerAvatar : public APangeaCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerAvatar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Pangea|PlayerAvatar")
	void AttachWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Pangea|PlayerAvatar")
	void DropWeapon(AWeapon* Weapon);

	virtual void Attack() override;

	FORCEINLINE class UCameraComponent* GetCameraComponent() const {return _cameraComponent; }
	
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() const {return _springArmComponent; }


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _springArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* _cameraComponent;
	

};
