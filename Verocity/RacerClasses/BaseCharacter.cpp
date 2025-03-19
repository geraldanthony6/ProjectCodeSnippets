// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//#include "ToolBuilderUtil.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h" 
#include "RaceCheckpoint.h"
#include "RaceManager.h"
#include "Abilities/BananaTrap.h"
#include "Character/CharacterAbilitySystemComponent.h"
#include "Character/CharacterAttributeSetBase.h"
#include "Character/CharacterGameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAlwaysRelevant = true;
	SetReplicates(true);
	bReplicates = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Overlap);
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));

	NiagaraSystemComponentOne = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponentOne"));
	NiagaraSystemComponentOne->SetupAttachment(RootComponent);
	NiagaraSystemComponentOne->SetAutoActivate(false);

	NiagaraSystemComponentTwo = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponentTwo"));
	NiagaraSystemComponentTwo->SetupAttachment(RootComponent);
	NiagaraSystemComponentTwo->SetAutoActivate(false);

	NiagaraHeadSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraHeadSystemComponent"));
	NiagaraHeadSystemComponent->SetupAttachment(RootComponent);
	NiagaraHeadSystemComponent->SetAutoActivate(false);

	LeftSpeedRingComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LeftSpeedRing"));
	LeftSpeedRingComponent->SetupAttachment(RootComponent);
	LeftSpeedRingComponent->SetAutoActivate(false);

	RightSpeedRingComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RightSpeedRing"));
	RightSpeedRingComponent->SetupAttachment(RootComponent);
	RightSpeedRingComponent->SetAutoActivate(false);

	
	// Controller Config
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	GetCharacterMovement()->GroundFriction = 5.0f;

	// Camera Setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bUsePawnControlRotation = true;
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;

	SpeedBoostEffectNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpeedBoost"));
	SpeedBoostEffectNiagaraComponent->SetupAttachment(PlayerCamera);
	SpeedBoostEffectNiagaraComponent->SetAutoActivate(false);

	bIsAttacking = false;

	ItemTimer = 0.0f;

	
	/* Movement Init */
	MaxSpeed = 2000.0f;
	CurrentSpeed = 1000.0f;
	SlowSpeed = 100.0f;
	//GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

	/* Health Init */
	MaxHealth = 1000.0f;
	RacerHealth = MaxHealth;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnCharacterBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnCharacterEndOverlap);

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("RacerAbilitySystem"));

}

#pragma region "Ability System Stuff"
void ABaseCharacter::AddCharacterAbilities()
{
	/*
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for(TSubclassOf<UCharacterGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->CurAbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
	*/
}

void ABaseCharacter::InitializeAttributes()
{
	/*
	if(!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if(!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if(NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
	*/
}

void ABaseCharacter::AddStartupEffects()
{
	/*
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if(NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
	*/

}


bool ABaseCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}


int32 ABaseCharacter::GetAbilityLevel(AbilityID AbilityID) const
{
	return 1;
}

void ABaseCharacter::RemoveCharacterAbilities()
{
	/*
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for(const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for(int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
	*/
}

void ABaseCharacter::Die()
{
	/*
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
	*/
}

void ABaseCharacter::FinishDying()
{
	Destroy();
}

/*
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}
*/

#pragma endregion 

#pragma region "Begin/Tick/Replicated"

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentItem = EItemTypes::None;

	if(HasAuthority())
	{
		bIsHost = true;
		Server_SetRacerPosition(1);
	}
	else
	{
		Server_SetRacerPosition(0);
	}

	/*
	if(AbilitySystem)
	{
		if(HasAuthority() && CharacterAbilities)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(CharacterAbilities.GetDefaultObject(), 1, 0));
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
	*/

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bIsHost);
	
	DOREPLIFETIME(ABaseCharacter, CharacterRacePosition);

	/* Replicate Speed */
	DOREPLIFETIME(ABaseCharacter, CurrentSpeed);

	/* Replicate Shield Bool */
	DOREPLIFETIME(ABaseCharacter, bIsShieldActive);

	DOREPLIFETIME(ABaseCharacter, bIsSlowed);

	DOREPLIFETIME(ABaseCharacter, SlowTimer);

	/* Replicate Attack Variables */
	DOREPLIFETIME(ABaseCharacter, bIsAttacking);
	DOREPLIFETIME(ABaseCharacter, AttackFieldTimer);

	DOREPLIFETIME(ABaseCharacter, CurrentCheckpoint);

	DOREPLIFETIME(ABaseCharacter, RacerID);

	DOREPLIFETIME(ABaseCharacter, bUsedBulldozer);
	DOREPLIFETIME(ABaseCharacter, bBulldozerGrowthComplete);

	//DOREPLIFETIME(ABaseCharacter, GroundFrictionValue);

	DOREPLIFETIME(ABaseCharacter, RacerHealth);

	DOREPLIFETIME(ABaseCharacter, SlowSpeed);

	DOREPLIFETIME(ABaseCharacter, ShieldTimer);

	DOREPLIFETIME(ABaseCharacter, ItemTimer);

	DOREPLIFETIME(ABaseCharacter, JumpTimeLimit);

	DOREPLIFETIME(ABaseCharacter, FrogJumps);

	DOREPLIFETIME(ABaseCharacter, MonkeyJumps);

	DOREPLIFETIME(ABaseCharacter, bHighJumpActive);

	DOREPLIFETIME(ABaseCharacter, bHasFastRecovery);

	DOREPLIFETIME(ABaseCharacter, HeadCooldown);

	DOREPLIFETIME(ABaseCharacter, ChestCooldown);

	DOREPLIFETIME(ABaseCharacter, LegCooldown);

	DOREPLIFETIME(ABaseCharacter, UltimateCooldown);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetCharacterMovement()->Velocity == FVector(0.0f))
	{
		LeftSpeedRingComponent->Deactivate();
		RightSpeedRingComponent->Deactivate();
	}

	if(bIsSlowed && SlowTimer > 0.0f)
	{
		SlowTimer -= DeltaTime;
	}

	if(bIsSlowed && SlowTimer <= 0.0f)
	{
		bIsSlowed = false;
		GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
		/*
		BaseCharacterSkeletalMesh->SetMaterial(0, BaseMaterialOne);
		BaseCharacterSkeletalMesh->SetMaterial(1, BaseMaterialTwo);
		*/
	}

	// Chest related abilities
	if(ChestCooldown > 0.0f)
	{
		ChestCooldown -= DeltaTime;
	}

	if(ChestCooldown <= 0.0f)
	{
		//bUsedBulldozer = false;
		GetMesh()->SetMaterial(0, BaseMaterialOne);
		GetMesh()->SetMaterial(0, BaseMaterialTwo);
		GetMesh()->SetOverlayMaterial(nullptr);
		NiagaraSystemComponentOne->Deactivate();
		NiagaraSystemComponentTwo->Deactivate();
	}

	if(bUsedBulldozer && PlayerScalingAmount < 2.0f)
	{
		PlayerScalingAmount += DeltaTime;
		SetActorScale3D(FVector(PlayerScalingAmount));
	} else if(bUsedBulldozer && PlayerScalingAmount >= 2.0f)
	{
		//GetCapsuleComponent()->SetCapsuleRadius(150.0f);
		LegCooldown = 4.0f;
		bBulldozerGrowthComplete = true;
		bUsedBulldozer = false;
	}

	if(bBulldozerGrowthComplete && PlayerScalingAmount > 1.0f && LegCooldown <= 0.0f)
	{
		PlayerScalingAmount -= DeltaTime;
		SetActorScale3D(FVector(PlayerScalingAmount));
	} else if(bBulldozerGrowthComplete && PlayerScalingAmount <= 1.0f && LegCooldown <= 0.0f)
	{
		bUsedBulldozer = false;
		bBulldozerGrowthComplete = false;
		PlayerScalingAmount = 1.0f;
		GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	}

	if(LegCooldown > 0.0f)
	{
		LegCooldown -= DeltaTime;
		LegCooldown -= DeltaTime;
	}

	if(LegCooldown <= 0.0f)
	{
		//SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if(JumpTimeLimit > 0.0f)
	{
		JumpTimeLimit -= DeltaTime;
	}

	if(JumpTimeLimit <= 0.0f  && bIsFrogLegs && FrogJumps > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
		FrogJumps = 0;
		LegCooldown = 6.0f;
	}

	if(JumpTimeLimit <= 0.0f  && bIsMonkeyLegs && MonkeyJumps > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
		MonkeyJumps = 0;
		LegCooldown = 6.0f;
	}

	if(UltimateCooldown > 0.0f)
	{
		UltimateCooldown -= DeltaTime;
	}

	if(ItemTimer > 0.0f)
	{
		ItemTimer -= DeltaTime;
	}

	if(bHasSpeedBoost && SpeedBoostTimer > 0.0f)
	{
		SpeedBoostTimer -= DeltaTime;
	}

	if(bHasSpeedBoost && SpeedBoostTimer <= 0.0f)
	{
		Server_UpdateCharacterSpeed(2000.0f, 1000.0f);
		Server_UpdateCharacterSpeedLines(false);
		bHasSpeedBoost = false;
	}

	
}

#pragma endregion 

#pragma region "Getters and Setters"
void ABaseCharacter::SetHealth(float Health)
{
	if(AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void ABaseCharacter::SetSpeed(float Speed)
{
	if(AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetSpeed(Speed);
	}
}

void ABaseCharacter::SetRacerPosition(int NewPosition)
{
	CharacterRacePosition = NewPosition;
}

void ABaseCharacter::Server_SetRacerPosition_Implementation(int NewPosition)
{
	Multicast_SetRacerPosition(NewPosition);
	UpdateRacerPosition(CharacterRacePosition);
}

void ABaseCharacter::Multicast_SetRacerPosition_Implementation(int NewPosition)
{
	CharacterRacePosition = NewPosition;
}


void ABaseCharacter::UpdateRacerPosition(int NewPosition)
{
	CharacterRacePosition = NewPosition;	
}



void ABaseCharacter::SetIsSlowed(bool newSlow)
{
	bIsSlowed = newSlow;
}

void ABaseCharacter::SetSlowTimer(float newSlowTimer)
{
	SlowTimer = newSlowTimer;
}

void ABaseCharacter::SetCurID(int ID)
{
	RacerID = ID;
}

int ABaseCharacter::GetCurID()
{
	return RacerID;
}

void ABaseCharacter::OnRep_CurrentSpeed()
{
	UE_LOG(LogTemp, Warning, TEXT("This dudes speed should change"));
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

void ABaseCharacter::OnRep_CharacterRacePosition()
{
	//UpdateRacerPosition()
}

void ABaseCharacter::OnRep_ShieldActive(bool ShieldStatus)
{
	bIsShieldActive = ShieldStatus;
	ShieldTimer = 5.0f;
}

float ABaseCharacter::GetCharacterLevel() const
{
	return AttributeSetBase->GetLevel();
}

float ABaseCharacter::GetHealth() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float ABaseCharacter::GetMaxHealth() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float ABaseCharacter::GetSpeed() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetSpeed();
	}

	return 0.0f;
}

float ABaseCharacter::GetMaxSpeed() const
{

	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxSpeed();
	}

	return 0.0f;
}
#pragma endregion 

#pragma region "Items"

void ABaseCharacter::HandleUsingItem()
{
	if(bHasOilSpill)
	{
		Server_OilSpillItem();
	}

	if(bHasDroneStrike)
	{
		Server_DroneStrikeItem();
	}

	if(bHasElectricShock)
	{
		Server_ElectricShockItem();
	}

	if(bHasFakeItemBox)
	{
		Server_FakeItemBoxItem();
		
	}

	if(bHasEnergyBlaster)
	{
		Server_EnergyBlasterItem();
	}

	if(bHasRobotRatStealer)
	{
		Server_RobotRatItem();
	}
}

void ABaseCharacter::AttachItem(EItemTypes Item)
{
	CurrentItem = Item;
	switch (Item)
	{
	case EItemTypes::None:
		ResetAllItemTags();
		Handle_ItemUI_Update(EItemTypes::None);
		break;
	case EItemTypes::OilSpill:
		UE_LOG(LogTemp, Warning, TEXT("Attach oil spill Item"));
		ResetAllItemTags();
		bHasOilSpill = true;
		Handle_ItemUI_Update(EItemTypes::OilSpill);
		break;
	case EItemTypes::DroneStrike:
		UE_LOG(LogTemp, Warning, TEXT("Attach drone strike Item"));
		ResetAllItemTags();
		bHasDroneStrike = true;
		Handle_ItemUI_Update(EItemTypes::DroneStrike);
		break;
	case EItemTypes::ElectricShock:
		UE_LOG(LogTemp, Warning, TEXT("Attach electric shock Item"));
		ResetAllItemTags();
		bHasElectricShock = true;
		Handle_ItemUI_Update(EItemTypes::ElectricShock);
		break;
	case EItemTypes::FakeItemBox:
		UE_LOG(LogTemp, Warning, TEXT("Attach fake item box Item"));
		ResetAllItemTags();
		bHasFakeItemBox = true;
		Handle_ItemUI_Update(EItemTypes::FakeItemBox);
		break;
	case EItemTypes::EnergyBlaster:
		UE_LOG(LogTemp, Warning, TEXT("Attach energy blaster Item"));
		ResetAllItemTags();
		bHasEnergyBlaster = true;
		Handle_ItemUI_Update(EItemTypes::EnergyBlaster);
		break;
	case EItemTypes::RobotRatStealer:
		UE_LOG(LogTemp, Warning, TEXT("Attach robot rat Item"));
		ResetAllItemTags();
		bHasRobotRatStealer = true;
		Handle_ItemUI_Update(EItemTypes::RobotRatStealer);
		break;
	case EItemTypes::FakeBox:
		UE_LOG(LogTemp, Warning, TEXT("Be stunned"));
		Stun();
		break;
	default:
		break;
	}
}

void ABaseCharacter::ResetAllItemTags()
{
	bHasOilSpill = false;
	bHasDroneStrike = false;
	bHasElectricShock = false;
	bHasFakeItemBox = false;
	bHasEnergyBlaster = false;
	bHasRobotRatStealer =  false;
}

void ABaseCharacter::Server_OilSpillItem_Implementation()
{
	Multicast_OilSpillItem();
}

void ABaseCharacter::Multicast_OilSpillItem_Implementation()
{
	if(!bIsAiming)
	{
		ToggleCrosshair();
	}
	else if(bIsAiming)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use oil spill!"));
		// Place oil spill behind player

		UWorld* World = GetWorld();

		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + (GetActorForwardVector() * -1500.0f) + FVector(0.0f, 0.0f, -250.0f);
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, nullptr);

		bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

		DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, 2.0f, 0, 1.0f);

		//Server_PlayMontage(MonkeyTrapMontage);

		if(bHit)
		{
			FVector SpawnPoint = HitResult.Location + FVector(0.0f, 0.0f, 10.0f);
			FActorSpawnParameters TrapSpawnParameters;
		
			AActor* NewTrap = GetWorld()->SpawnActor<AActor>(OilSpillTrap, SpawnPoint, FRotator::ZeroRotator, TrapSpawnParameters);
		}

		AttachItem(EItemTypes::None);

		ToggleCrosshair();
	}

	
}

void ABaseCharacter::Server_DroneStrikeItem_Implementation()
{
	Multicast_DroneStrikeItem();
}

void ABaseCharacter::Multicast_DroneStrikeItem_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use drone strike!"));

	TSubclassOf<ABaseCharacter> RacerClass = ABaseCharacter::GetClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RacerClass, FoundActors);

	for (AActor* CurrentActor : FoundActors)
	{
		ABaseCharacter* TempRacer = Cast<ABaseCharacter>(CurrentActor);
		if(TempRacer != this && !CurrentRacers.Contains(TempRacer))
		{
			CurrentRacers.Add(TempRacer);
		}
	}

	for (ABaseCharacter* CurrentRacer : CurrentRacers)
	{
		if(CurrentRacer->GetRacerPosition() == 1)
		{
			CurrentRacer->Stun(2.0f);
		}
	}
	
	AttachItem(EItemTypes::None);
}

void ABaseCharacter::Server_ElectricShockItem_Implementation()
{
	Multicast_ElectricShockItem();
}

void ABaseCharacter::Multicast_ElectricShockItem_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use electric shock!"));

	TSubclassOf<ABaseCharacter> RacerClass = ABaseCharacter::GetClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RacerClass, FoundActors);

	for (AActor* CurrentActor : FoundActors)
	{
		ABaseCharacter* TempRacer = Cast<ABaseCharacter>(CurrentActor);
		if(TempRacer != this && !CurrentRacers.Contains(TempRacer))
		{
			CurrentRacers.Add(TempRacer);
		}
	}

	for (ABaseCharacter* CurrentRacer : CurrentRacers)
	{
		CurrentRacer->Stun();
	}

	AttachItem(EItemTypes::None);
}

void ABaseCharacter::Server_FakeItemBoxItem_Implementation()
{
	Multicast_FakeItemBoxItem();
}

void ABaseCharacter::Multicast_FakeItemBoxItem_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use fake item box!"));

	
	UWorld* World = GetWorld();

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() * -1500.0f) + FVector(0.0f, 0.0f, -250.0f);
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, nullptr);

	bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, 2.0f, 0, 1.0f);

	//Server_PlayMontage(MonkeyTrapMontage);

	if(bHit)
	{
		FVector SpawnPoint = HitResult.Location + FVector(0.0f, 0.0f, 10.0f);
		FActorSpawnParameters TrapSpawnParameters;
		
		AActor* NewTrap = GetWorld()->SpawnActor<AActor>(FakeItemBox, SpawnPoint, FRotator::ZeroRotator, TrapSpawnParameters);
		AMysteryBox* NewFakeBox = Cast<AMysteryBox>(NewTrap);
		NewFakeBox->SetIsFake(true);
	}

	AttachItem(EItemTypes::None);
}

void ABaseCharacter::Server_EnergyBlasterItem_Implementation()
{
	Multicast_EnergyBlasterItem();
}

void ABaseCharacter::Multicast_EnergyBlasterItem_Implementation()
{
	if(!bIsAiming)
	{
		ToggleCrosshair();
	}
	else if(bIsAiming)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use energy blaster!"));

		// Send out a ray that can hit the person in front of you and stun them
		UWorld* World = GetWorld();

		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + GetActorForwardVector() * 1000.0f;
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, nullptr);

		bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

		DrawDebugLine(World, StartLocation, EndLocation, FColor::Green, false, 2.0f, 0, 1.0f);

		//Server_PlayMontage(FrogGrabMontage);

		if(bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Hit someone!"));
			AActor* HitActor = HitResult.GetActor();
			if(Cast<ABaseCharacter>(HitActor))
			{
				ABaseCharacter* HitRacer = Cast<ABaseCharacter>(HitActor);
				HitRacer->Stun();
			}
		}

		AttachItem(EItemTypes::None);

		ToggleCrosshair();
	}

}

void ABaseCharacter::Server_RobotRatItem_Implementation()
{
	Multicast_RobotRatItem();
}

void ABaseCharacter::Multicast_RobotRatItem_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use robo rat!"));

	TSubclassOf<ABaseCharacter> RacerClass = ABaseCharacter::GetClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RacerClass, FoundActors);

	for (AActor* CurrentActor : FoundActors)
	{
		ABaseCharacter* TempRacer = Cast<ABaseCharacter>(CurrentActor);
		if(TempRacer != this && !CurrentRacers.Contains(TempRacer))
		{
			CurrentRacers.Add(TempRacer);
		}
	}

	for (ABaseCharacter* CurrentRacer : CurrentRacers)
	{
		if(CurrentRacer->CurrentItem != EItemTypes::None)
		{
			AttachItem(CurrentRacer->CurrentItem);
			CurrentRacer->AttachItem(EItemTypes::None);
		}
	}

	AttachItem(EItemTypes::None);
}

#pragma endregion

#pragma region "Character Helpers"

void ABaseCharacter::Stun(float TimeToStun)
{
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	SlowTimer = TimeToStun;
	bIsSlowed = true;
}

void ABaseCharacter::Server_UpdateOtherRacerSpeed_Implementation(float newSpeed, ABaseCharacter* OtherRacer, bool bSlowed, float SlowTime)
{
	Multi_UpdateOtherRacerSpeed(newSpeed, OtherRacer, bSlowed, SlowTime);
}

void ABaseCharacter::Multi_UpdateOtherRacerSpeed_Implementation(float newSpeed, ABaseCharacter* OtherRacer, bool bSlowed, float SlowTime)
{
	OtherRacer->GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	
	if(bSlowed)
	{
		OtherRacer->SetIsSlowed(true);
		OtherRacer->SetSlowTimer(SlowTime);
		//OtherRacer->BaseCharacterSkeletalMesh->SetMaterial(0, SlowedMaterial);
		//OtherRacer->BaseCharacterSkeletalMesh->SetMaterial(1, SlowedMaterial);
	}
}

void ABaseCharacter::Server_PlayMontage_Implementation(UAnimMontage* MontageToPlay, float PlayRate)
{
	Multicast_PlayMontage(MontageToPlay, PlayRate);
}

void ABaseCharacter::Multicast_PlayMontage_Implementation(UAnimMontage* MontageToPlay, float PlayRate)
{
	this->PlayAnimMontage(MontageToPlay);
}

#pragma endregion

#pragma region "Character Collision"

void ABaseCharacter::OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bIsAttacking)
	{
			ABaseCharacter* HitActor = Cast<ABaseCharacter>(OtherActor);
			if(HitActor && !HitActor->bIsShieldActive)
			{
				if(HasAuthority())
				{
					UE_LOG(LogTemp, Warning, TEXT("Server hit someone another dude"));
					Multi_UpdateOtherRacerSpeed(SlowSpeed, HitActor, true, 3.0f);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Client hit another dude"));
					Server_UpdateOtherRacerSpeed(SlowSpeed, HitActor, true, 3.0f);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't attack they're shielding"));
			}
	}
}

void ABaseCharacter::OnAttackEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ABaseCharacter::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Checkpoint"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint Hit"));
		CurrentCheckpoint = Cast<ARaceCheckpoint>(OtherActor)->CheckpointNum;
		RaceManager->Server_UpdateRacers_Implementation(RacerID, CurrentCheckpoint);
	}

	if(OtherActor->ActorHasTag("OilSpill"))
	{
		if(HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit a Spill!"));
			Multi_UpdateOtherRacerSpeed(SlowSpeed, this, true, 1.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit a Spill!"));
			Server_UpdateOtherRacerSpeed(SlowSpeed, this, true, 1.0f);
		}
	}

	if(OtherActor->ActorHasTag("MysteryBox") && ItemTimer <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get an item from the mystery box you hit bud"));
		// Put the item you get into the players item slot
		AMysteryBox* HitBox = Cast<AMysteryBox>(OtherActor);
		EItemTypes HitItem = HitBox->GetItemType();
		AttachItem(HitItem);
		HitBox->BeDestroyed();
		ItemTimer = 2.0f;
	}

	if(OtherActor->ActorHasTag("BananaTrap"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString(TEXT("Add Slow or Stun debuff")));
		Stun(1.5f);
	}
}

void ABaseCharacter::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

#pragma endregion 

#pragma region "Character Input"

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::TurnPlayer);

		//EnhancedInputComponent->BindAction(CameraLookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::CameraLook);

		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ABaseCharacter::HandleBrakeTrigger);

		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Canceled, this, &ABaseCharacter::HandleBrakeRelease);

		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ABaseCharacter::HandleBrakeRelease);
		
		EnhancedInputComponent->BindAction(HeadAction, ETriggerEvent::Started, this, &ABaseCharacter::HandleHeadAbility);

		EnhancedInputComponent->BindAction(BodyAction, ETriggerEvent::Started, this, &ABaseCharacter::HandleBodyAbility);

		EnhancedInputComponent->BindAction(LegAction, ETriggerEvent::Started, this, &ABaseCharacter::HandleLegAbility);

		EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Started, this, &ABaseCharacter::HandleUltimateAbility);

		EnhancedInputComponent->BindAction(ItemAction, ETriggerEvent::Started, this, &ABaseCharacter::HandleUsingItem);
	}

}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		LeftSpeedRingComponent->Activate();
		RightSpeedRingComponent->Activate();
		
		// find the forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get Forward Vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add Movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::TurnPlayer(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
}

void ABaseCharacter::CameraLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABaseCharacter::HandleBrakeTrigger(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Start braking/drifting"));
	Server_UpdateCharacterFriction(0.5, false);
	Server_UpdateCharacterSpeed(1700.0f, 1000.0f);
}

void ABaseCharacter::HandleBrakeRelease(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Stop braking/drifting"));
	Server_UpdateCharacterFriction(5.0f, true);
	if(SpeedBuildUp >= 1.0f)
	{
		Server_UpdateCharacterSpeedTemp(4000.0f, 4000.0f);
	} else if(SpeedBuildUp < 1.0f)
	{
		Server_UpdateCharacterSpeed(2000.0f, 1000.0f);
	}
}

void ABaseCharacter::Server_UpdateCharacterFriction_Implementation(float NewFriction, bool bIsLetGo)
{
	Multicast_UpdateCharacterFriction(NewFriction, bIsLetGo);
}

void ABaseCharacter::Multicast_UpdateCharacterFriction_Implementation(float NewFriction, bool bIsLetGo)
{
	GetCharacterMovement()->GroundFriction = NewFriction;
	if(!bIsLetGo)
	{
		SpeedBuildUp += 0.01f;
	}
	else if(bIsLetGo)
	{
		SpeedBuildUp = 0.0f;
	}
}

void ABaseCharacter::Server_UpdateCharacterSpeed_Implementation(float NewSpeed, float NewAcceleration)
{
	Multicast_UpdateCharacterSpeed(NewSpeed, NewAcceleration);
}

void ABaseCharacter::Multicast_UpdateCharacterSpeed_Implementation(float NewSpeed, float NewAcceleration)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	GetCharacterMovement()->MaxAcceleration = NewAcceleration;
}

void ABaseCharacter::Server_UpdateCharacterSpeedTemp_Implementation(float NewSpeed, float NewAcceleration)
{
	Multicast_UpdateCharacterSpeedTemp(NewSpeed, NewAcceleration);
}

void ABaseCharacter::Multicast_UpdateCharacterSpeedTemp_Implementation(float NewSpeed, float NewAcceleration)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	GetCharacterMovement()->MaxAcceleration = NewAcceleration;
	Server_UpdateCharacterSpeedLines(true);
	bHasSpeedBoost = true;
	SpeedBoostTimer = 1.5f;
}

void ABaseCharacter::Server_UpdateCharacterSpeedLines_Implementation(bool bTurnOn)
{
	Client_UpdateCharacterSpeedLines(bTurnOn);
}

void ABaseCharacter::Client_UpdateCharacterSpeedLines_Implementation(bool bTurnOn)
{
	if(bTurnOn)
	{
		SpeedBoostEffectNiagaraComponent->Activate();
	} else if(!bTurnOn)
	{
		SpeedBoostEffectNiagaraComponent->Deactivate();
	}
}

#pragma endregion

#pragma region "Character Abilities"

void ABaseCharacter::HandleHeadAbility()
{
	if(bIsJaguarHead)
	{
		Server_Bite();
	}

	if(bIsFrogHead)
	{
		Server_GrappleTongue();
	}

	if(bIsMonkeyHead)
	{
		Server_FastRecovery();
	}
}

void ABaseCharacter::HandleBodyAbility()
{
	if(bIsJaguarBody)
	{
		Server_Invisible();
	}

	if(bIsFrogBody)
	{
		Server_PoisionTouch();
	}

	if(bIsMonkeyBody)
	{
		Server_FireTraps();
	}
}

void ABaseCharacter::HandleLegAbility()
{
	if(bIsJaguarLegs)
	{
		Server_Bulldozer();
	}

	if(bIsFrogLegs)
	{
		Server_MegaLeap();
	}

	if(bIsMonkeyLegs)
	{
		Server_MonkeyPounce();
	}
}

void ABaseCharacter::HandleUltimateAbility()
{
	if(bHasJaguarUltimate)
	{
		Server_JaguarUltimate();
	}

	if(bHasFrogUltimate)
	{
		Server_FrogUltimate();
	}

	if(bHasMonkeyUltimate)
	{
		Server_MonkeyUltimate();
	}
}

void ABaseCharacter::ResetAllAbilityTags(int ID)
{
	switch (ID)
	{
	case 0:
		bIsJaguarHead = false;
		bIsFrogHead = false;
		bIsMonkeyHead = false;
		break;
	case 1:
		bIsJaguarBody = false;
		bIsFrogBody = false;
		bIsMonkeyBody = false;
		break;
	case 2:
		bIsJaguarLegs = false;
		bIsFrogLegs = false;
		bIsMonkeyLegs = false;
		break;
	case 3:
		bHasJaguarUltimate = false;
		bHasFrogUltimate = false;
		bHasMonkeyUltimate = false;
		break;
	default:
	break;	
	}
}

void ABaseCharacter::ThrowBanana(FVector Direction, FVector LineOffset, bool bIsUltimate)
{
	if(bIsUltimate)
	{
		// Fire Traps
		UWorld* World = GetWorld();

		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + Direction + LineOffset;
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, nullptr);

		bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

		DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, 2.0f, 0, 1.0f);

		if(bHit)
		{
			FVector SpawnPoint = HitResult.Location + FVector(0.0f, 0.0f, 10.0f);
			FActorSpawnParameters TrapSpawnParameters;
		
			ABananaTrap* NewTrap = GetWorld()->SpawnActor<ABananaTrap>(BananaTrapBlueprint, SpawnPoint, FRotator::ZeroRotator, TrapSpawnParameters);

			FVector ForceToAdd = FVector(10000.0f, 0.0f, 0.0f);
		
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(NewTrap);

			if(PrimitiveComponent)
			{
				PrimitiveComponent->AddForce(ForceToAdd, NAME_None);
			}
		}
	}
	else
	{
		// Fire Traps
		if(!bIsAiming)
		{
			ToggleCrosshair();
		} else if(bIsAiming)
		{
			UWorld* World = GetWorld();

			FVector StartLocation = GetActorLocation();
			FVector EndLocation = StartLocation + Direction + LineOffset;
			FVector EndLocationTemp = StartLocation + (GetActorRightVector() * 1000.0f) + FVector(0.0f, 0.0f, -250.0f);
			FHitResult HitResult;
			FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, nullptr);

			bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocationTemp, ECC_Visibility, TraceParams);

			DrawDebugLine(World, StartLocation, EndLocationTemp, FColor::Blue, false, 2.0f, 0, 1.0f);

			Server_PlayMontage(MonkeyTrapMontage);

			if(bHit)
			{
				FVector SpawnPoint = HitResult.Location + FVector(0.0f, 0.0f, 10.0f);
				FActorSpawnParameters TrapSpawnParameters;
		
				ABananaTrap* NewTrap = GetWorld()->SpawnActor<ABananaTrap>(BananaTrapBlueprint, SpawnPoint, FRotator::ZeroRotator, TrapSpawnParameters);

				FVector ForceToAdd = FVector(10000.0f, 0.0f, 0.0f);
		
				UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(NewTrap);

				if(PrimitiveComponent)
				{
					PrimitiveComponent->AddForce(ForceToAdd, NAME_None);
				}
			}
			ToggleCrosshair();
		}

	}

}

void ABaseCharacter::Server_FastRecovery_Implementation()
{
	Multicast_FastRecovery();
}

void ABaseCharacter::Multicast_FastRecovery_Implementation()
{
	// Adjust player's attribute that is related to recovery
	// Set a bool stating that this character's recovery time is quicker
	bHasFastRecovery = true;
}

void ABaseCharacter::Server_FireTraps_Implementation()
{
	Multicast_FireTraps();
}

void ABaseCharacter::Multicast_FireTraps_Implementation()
{
	ThrowBanana((GetActorRightVector() * 1000.0f), FVector(0.0f, 0.0f, -250.0f), false);
}

void ABaseCharacter::Server_MonkeyPounce_Implementation()
{
	Multicast_MonkeyPounce();
}

void ABaseCharacter::Multicast_MonkeyPounce_Implementation()
{
	// Add more speed to the monkey for a short period of time
	if(MonkeyJumps < 3 && LegCooldown <= 0.0f && JumpTimeLimit <= 0.0f)
	{
		GetCharacterMovement()->JumpZVelocity = 1500.0f;
		GetCharacterMovement()->MaxWalkSpeed = 4000.0f;
		GetCharacterMovement()->DoJump(true);
		MonkeyJumps++;
		JumpTimeLimit = 6.0f;
	}else if (MonkeyJumps < 3 && LegCooldown <= 0.0f && JumpTimeLimit > 0.0f)
	{
		GetCharacterMovement()->JumpZVelocity = 1500.0f;
		GetCharacterMovement()->DoJump(true);
		MonkeyJumps++;
	}
}

void ABaseCharacter::Server_MonkeyUltimate_Implementation()
{
	Multicast_MonkeyUltimate();
}

void ABaseCharacter::Multicast_MonkeyUltimate_Implementation()
{
	if(UltimateCooldown <= 0.0f)
	{
		UltimateCooldown = 30.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString(TEXT("Use monkey ult")));
		for(int i = 0; i < 7; i++)
		{
			switch (i)
			{
			case 0:
				ThrowBanana( (GetActorForwardVector() * -1000), FVector(0.0, 0.0f, -250.0f), true);
				break;
			case 1:
				ThrowBanana( (GetActorForwardVector() * -1000), FVector(0.0, 250.0f, -250.0f), true);
				break;	
			case 2:
				ThrowBanana(GetActorRightVector() * 1000.0f, FVector(0.0f, 0.0f, -250.0f), true);
				break;
			case 3:
				ThrowBanana(GetActorRightVector() * -1000.0f, FVector(0.0f, 0.0f, -250.0f), true);
				break;
			case 4:
				ThrowBanana(GetActorRightVector() * -1000.0f, FVector(-400.0f, 0.0f, -250.0f), true);
				break;
			case 5:
				ThrowBanana(GetActorRightVector() * 1000.0f, FVector(-400.0f, 0.0f, -250.0f), true);
				break;
			case 6:
				//ThrowBanana(GetActorForwardVector() * 1000.0f, FVector(-250.0f, 0.0f, -250.0f), true);
					break;
			default:
				break;	
			}
		}
	}
}

void ABaseCharacter::Server_GrappleTongue_Implementation()
{
	Multicast_GrappleTongue();
}

void ABaseCharacter::Multicast_GrappleTongue_Implementation()
{
	if(!bIsAiming)
	{
		ToggleCrosshair();
	}else if (bIsAiming)
	{
		UWorld* World = GetWorld();

		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + GetActorForwardVector() * 1000.0f;
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, nullptr);

		bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

		DrawDebugLine(World, StartLocation, EndLocation, FColor::Green, false, 2.0f, 0, 1.0f);

		Server_PlayMontage(FrogGrabMontage);
		NiagaraHeadSystemComponent->SetAsset(GrappleTongueComponent);
		NiagaraHeadSystemComponent->Activate();

		if(bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if(HitActor->ActorHasTag("Grab"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString(TEXT("Pull this actor")));
				FVector GrabOffset = GetActorLocation() + GetActorForwardVector() * 200.0f;
				HitActor->SetActorLocation(GrabOffset);
			}
		}
		ToggleCrosshair();
	}

}

void ABaseCharacter::Server_PoisionTouch_Implementation()
{
	Multicast_PoisionTouch();
}

void ABaseCharacter::Multicast_PoisionTouch_Implementation()
{
	NiagaraSystemComponentOne->SetAsset(PoisonNiagaraComponent);
	NiagaraSystemComponentOne->Activate();
	NiagaraSystemComponentTwo->SetAsset(PoisonRainNiagaraComponent);
	NiagaraSystemComponentTwo->Activate();
	GetMesh()->SetOverlayMaterial(PoisonMaterial);
	ChestCooldown = 3.0f;
}

void ABaseCharacter::Server_MegaLeap_Implementation()
{
	Multicast_MegaLeap();
}

void ABaseCharacter::Multicast_MegaLeap_Implementation()
{
	if(FrogJumps < 3 && LegCooldown <= 0.0f && JumpTimeLimit <= 0.0f)
	{
		GetCharacterMovement()->JumpZVelocity = 1500.0f;
		GetCharacterMovement()->MaxWalkSpeed = 4000.0f;
		GetCharacterMovement()->DoJump(true);
		FrogJumps++;
		JumpTimeLimit = 6.0f;
	}else if (FrogJumps < 3 && LegCooldown <= 0.0f && JumpTimeLimit > 0.0f)
	{
		GetCharacterMovement()->JumpZVelocity = 1500.0f;
		GetCharacterMovement()->DoJump(true);
		FrogJumps++;
	}

}

void ABaseCharacter::Server_FrogUltimate_Implementation()
{
	Multicast_FrogUltimate();
}

void ABaseCharacter::Multicast_FrogUltimate_Implementation()
{
	if(UltimateCooldown <= 0.0f)
	{
		UltimateCooldown = 30.0f;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use frog Poison Blast!"));

		TSubclassOf<ABaseCharacter> RacerClass = ABaseCharacter::GetClass();
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), RacerClass, FoundActors);

		for (AActor* CurrentActor : FoundActors)
		{
			ABaseCharacter* TempRacer = Cast<ABaseCharacter>(CurrentActor);
			if(TempRacer != this && !CurrentRacers.Contains(TempRacer))
			{
				TempRacer->Stun(3.0f);
			}
		}
	}

}

void ABaseCharacter::Server_Bite_Implementation()
{
	Multicast_Bite();
}

void ABaseCharacter::Multicast_Bite_Implementation()
{
	FVector SphereLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
	TArray<AActor*> HitActors;
	TArray<AActor*> ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TSubclassOf<AActor> ActorClassFilter = nullptr; 
	bool Hit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SphereLocation, 100.0f, ObjectTypes, ActorClassFilter,ActorsToIgnore, HitActors);

	if(Hit)
	{
		/*
		for (AActor* HitActor : HitActors)
		{
			// Damage the hit actor
		}
		*/
	}
	
	DrawDebugSphere(GetWorld(), (GetActorLocation() + (GetActorForwardVector() * 100.0f)), 100.0f, 10.0f, FColor::Red, false, 1.0f);
	Server_PlayMontage(JaguarBiteMontage);
}

void ABaseCharacter::Server_Invisible_Implementation()
{
	Multicast_Invisible();
}

void ABaseCharacter::Multicast_Invisible_Implementation()
{
	GetMesh()->SetMaterial(0, InvisibleMaterial);
	GetMesh()->SetMaterial(1, InvisibleMaterial);
	ChestCooldown = 4.0f;
}

void ABaseCharacter::Server_Bulldozer_Implementation()
{
	Multicast_Bulldozer();
}

void ABaseCharacter::Multicast_Bulldozer_Implementation()
{
	bUsedBulldozer = true;
	NiagaraSystemComponentOne->SetAsset(JaguarBulldozerComponent);
	NiagaraSystemComponentOne->Activate();
}

void ABaseCharacter::Server_JaguarUltimate_Implementation()
{
	Multicast_JaguarUltimate();
}

void ABaseCharacter::Multicast_JaguarUltimate_Implementation()
{
	if(UltimateCooldown <= 0.0f)
	{
		UltimateCooldown = 30.0f;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Use jaguar roar!"));

		TSubclassOf<ABaseCharacter> RacerClass = ABaseCharacter::GetClass();
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), RacerClass, FoundActors);

		for (AActor* CurrentActor : FoundActors)
		{
			ABaseCharacter* TempRacer = Cast<ABaseCharacter>(CurrentActor);
			if(TempRacer != this && !CurrentRacers.Contains(TempRacer))
			{
				TempRacer->Stun(3.0f);
			}
		}
	}
}

#pragma endregion 







