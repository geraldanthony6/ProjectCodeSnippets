// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAvatarAnimInstance.h"

// Sets default values
// Sets default values
ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true; // Rotate character to moving direction
	characterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	characterMovement->bConstrainToPlane = true;
	characterMovement->bSnapToPlaneAtStart = true;

	// Create the camera spring arm
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArmComponent->SetupAttachment(RootComponent);		//Attach to the character root
	_springArmComponent->SetUsingAbsoluteRotation(true);		//Don't rotate the arm with the character
	_springArmComponent->TargetArmLength = 800.f;				//Set the arm's length 
	_springArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
																//Set the arm's rotation (60 degree up)
	_springArmComponent->bDoCollisionTest = false;				//No collision test

	// Create the camera
	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_cameraComponent->SetupAttachment(_springArmComponent, USpringArmComponent::SocketName);
																//Attach to the spring arm
	_cameraComponent->bUsePawnControlRotation = false;			//Camera rotation is not controllable  
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	_HealthPoints = HealthPoints;
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//auto animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	//animInst->Speed = GetCharacterMovement()->Velocity.Size2D();

	if (_AttackCountingDown == AttackInterval)
	{
		//animInst->State = EPlayerState::Attack;
	}
	
	if (_AttackCountingDown > 0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int ATestCharacter::GetHealthPoints()
{
	return _HealthPoints;
}


bool ATestCharacter::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}


bool ATestCharacter::CanAttack()
{
	//auto animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	//return (_AttackCountingDown <= 0.0f && animInst->State == EPlayerState::Locomotion);
	return true;
}


void ATestCharacter::Attack()
{
	_AttackCountingDown = AttackInterval;
}

void ATestCharacter::Hit(int damage)
{

}

void ATestCharacter::DieProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	Destroy();
	GEngine->ForceGarbageCollection(true);
}
