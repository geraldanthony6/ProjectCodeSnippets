// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatar.h"

#include "PangeaAnimInstance.h"
#include "PlayerAvatarAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerAvatar::APlayerAvatar()
{
	// Setup rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Setup movement component
	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	characterMovement->bConstrainToPlane = true;
	characterMovement->bSnapToPlaneAtStart = true;

	// Create camera spring arm
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArmComponent->SetupAttachment(RootComponent);
	_springArmComponent->SetUsingAbsoluteRotation(true);
	_springArmComponent->TargetArmLength = 800.0f;
	_springArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	_springArmComponent->bDoCollisionTest = false;

	// Create camera
	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camrea"));
	_cameraComponent->SetupAttachment(_springArmComponent, USpringArmComponent::SocketName);
	_cameraComponent->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//_AnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();

	if(_AttackCountingDown == AttackInterval)
	{
		_AnimInstance->State = ECharacterState::Attack;
	}

	if(_AttackCountingDown > 0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}

}

// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerAvatar::AttachWeapon(AWeapon* Weapon)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
}

void APlayerAvatar::DropWeapon(AWeapon* Weapon)
{
	TArray<AActor*> attachedActors;
	GetAttachedActors(attachedActors, true);
	for(int i = 0; i < attachedActors.Num(); i++)
	{
		attachedActors[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		attachedActors[i]->SetActorRotation(FQuat::Identity);
		AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);
		if(weapon != nullptr)
		{
			weapon->Holder;
		}
	}
}

void APlayerAvatar::Attack()
{
	APangeaCharacter::Attack();
}



