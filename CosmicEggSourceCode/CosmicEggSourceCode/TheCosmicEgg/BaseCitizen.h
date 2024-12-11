// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseArea.h"
#include "GameFramework/Character.h"
#include "BaseCitizen.generated.h"

UENUM(BlueprintType)
enum class ECitizenType : uint8
{
	NEUTRAL,
	LUCIFERIC,
	AHRIMANIC,
	CHRISTIC
};

UCLASS()
class THECOSMICEGG_API ABaseCitizen : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCitizen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Energy related variables
#pragma region "Energy Variables"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _NeutralEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _LucifericEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _AhrimanicEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ChristEnergy;
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _bIsInteracting;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECitizenType CurrentCitizenType;

#pragma region "Citizen Materials and Appearance"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* _CharacterAura;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* _NeutralMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* _LucifericMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* _AhrimanicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* _ChristMaterial;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ABaseArea> CurrentArea;


private:
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	float _MaxEnergy = 100.0f;

	float _DefaultEnergy = 20.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Handles adding energy to citizens when they perform a certain action
	void RecieveEnergyFromAction(float NeutralEnergyGiven, float LucifericEnergyGiven, float AhrimanicEnergyGiven, float ChristEnergyGiven);

	// Sets the current area the player is in, this area contains different interactable objects that
	// the citizen can perform actions on.
	void SetCurrentArea(TObjectPtr<ABaseArea> NewArea);

private:

	// Handle updating citizen type and type of aura they currently have based on their energy levels
	void UpdateCitizenType();
};
