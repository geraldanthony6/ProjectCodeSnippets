// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCitizen.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

#pragma region "Constructor, Begin Play, Tick"
// Sets default values
ABaseCitizen::ABaseCitizen()
{
	_CharacterAura = CreateDefaultSubobject<UStaticMeshComponent>("Character Aura");
	_CharacterAura->SetupAttachment(RootComponent);
	
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Variable Set-up
	CurrentCitizenType = ECitizenType::NEUTRAL;

	_NeutralEnergy = _DefaultEnergy;
	_LucifericEnergy = _DefaultEnergy;
	_AhrimanicEnergy = _DefaultEnergy;
	_ChristEnergy = _DefaultEnergy;

}

// Called when the game starts or when spawned
void ABaseCitizen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(_bIsInteracting)
	{
		UpdateCitizenType();	
	}

}
#pragma endregion

#pragma region "Energy Manipulation Functions"
void ABaseCitizen::RecieveEnergyFromAction(float NeutralEnergyGiven, float LucifericEnergyGiven,
	float AhrimanicEnergyGiven, float ChristEnergyGiven)
{
	_bIsInteracting = true;
	_NeutralEnergy += NeutralEnergyGiven;
	_LucifericEnergy += LucifericEnergyGiven;
	_AhrimanicEnergy += AhrimanicEnergyGiven;
	_ChristEnergy += ChristEnergyGiven;
}
#pragma endregion

#pragma region "Citizen Handlers"
void ABaseCitizen::SetCurrentArea(TObjectPtr<ABaseArea> NewArea)
{
	CurrentArea = NewArea;
}

void ABaseCitizen::UpdateCitizenType()
{
	if(_NeutralEnergy >= 50.0f)
	{
		CurrentCitizenType = ECitizenType::NEUTRAL;
		_CharacterAura->SetMaterial(0, _NeutralMaterial);
		_bIsInteracting = false;
	}

	if(_LucifericEnergy >= 50.0f)
	{
		CurrentCitizenType = ECitizenType::LUCIFERIC;
		_CharacterAura->SetMaterial(0, _LucifericMaterial);
		_bIsInteracting = false;
	}

	if(_AhrimanicEnergy >= 50.0f)
	{
		CurrentCitizenType = ECitizenType::AHRIMANIC;
		_CharacterAura->SetMaterial(0, _AhrimanicMaterial);
		_bIsInteracting = false;
	}

	if(_ChristEnergy >= 50.0f)
	{
		CurrentCitizenType = ECitizenType::CHRISTIC;
		_CharacterAura->SetMaterial(0, _ChristMaterial);
		_bIsInteracting = false;
	}
}
#pragma endregion 
