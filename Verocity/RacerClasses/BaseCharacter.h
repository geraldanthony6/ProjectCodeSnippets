// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MysteryBox.h"
#include "GameplayTask.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "Verocity/Verocity.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ABaseCharacter*, Character);

UENUM()
enum class AbilityInput
{
	UseAbility1 UMETA(DisplayName = "Use Head Ability"),
};

UCLASS()
class VEROCITY_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	/* Camera Boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* Player Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;
	
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Character")
	int CharacterRacePosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* FrogGrabMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* FrogLeapMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* FrogShieldMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* MonkeyPounceMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* MonkeyTrapMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* MonkeyRecoverMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* JaguarBiteMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* JaguarRampageMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* JaguarStealthMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> CharacterAbilities;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UAttributeSet> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	
	virtual void AddCharacterAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void SetHealth(float Health);
	virtual void SetSpeed(float Speed);

	UFUNCTION(Server, Reliable)
	void Server_SetRacerPosition(int NewPosition);
	void Server_SetRacerPosition_Implementation(int NewPosition);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetRacerPosition(int NewPosition);
	void Multicast_SetRacerPosition_Implementation(int NewPosition);
	
public:
	UFUNCTION()
	void UpdateRacerPosition(int NewPosition);

protected:
	
	

	/* Character Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseCharacterMesh;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* BaseCharacterSkeletalMesh;
	*/
	
	/* Shield Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* BaseMaterialOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* BaseMaterialTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* SlowedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* InvisibleMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* PoisonMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> NiagaraSystemComponentOne;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> NiagaraSystemComponentTwo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> NiagaraHeadSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> LeftSpeedRingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> RightSpeedRingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> SpeedBoostEffectNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> SpeedRing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> SpeedBoostEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> PoisonNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> PoisonRainNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> GrappleTongueComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> JaguarBulldozerComponent;

	/* Attack Field */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttackField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AttackFieldMesh;
	
	/* Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LegAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HeadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BodyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UltimateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Race, meta = (AllowPrivateAccess = "true"))
	class ARaceManager* RaceManager;
	
public:
	// Sets default values for this character's properties
	ABaseCharacter(const class FObjectInitializer& ObjectInitializer);


	
	UPROPERTY(BlueprintAssignable, Category = "Character")
	FCharacterDiedDelegate OnCharacterDied;
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual bool IsAlive() const;

	inline int GetRacerPosition()
	{
		return CharacterRacePosition;
	};

	UFUNCTION()
	void SetRacerPosition(int NewPosition);
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual int32 GetAbilityLevel(AbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void FinishDying();
	
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetCharacterLevel() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
	float GetMaxSpeed() const;

	// Item and Ability Spawnable Actors

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<class ABananaTrap> BananaTrapBlueprint;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class AActor> OilSpillTrap;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class AActor> Drone;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class AMysteryBox> FakeItemBox;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class AActor> EnergyBlast;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class AActor> Rat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helpers")
	TArray<ABaseCharacter*> CurrentRacers;

	/* Property Replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystem;
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Bools for ability tracking, will be changed after ability overhaul w/ GAS
	UPROPERTY(BlueprintReadWrite)
	bool bIsJaguarHead;
	UPROPERTY(BlueprintReadWrite)
	bool bIsFrogHead;
	UPROPERTY(BlueprintReadWrite)
	bool bIsMonkeyHead;

	UPROPERTY(BlueprintReadWrite)
	bool bIsJaguarBody;
	UPROPERTY(BlueprintReadWrite)
	bool bIsFrogBody;
	UPROPERTY(BlueprintReadWrite)
	bool bIsMonkeyBody;

	UPROPERTY(BlueprintReadWrite)
	bool bIsJaguarLegs;
	UPROPERTY(BlueprintReadWrite)
	bool bIsFrogLegs;
	UPROPERTY(BlueprintReadWrite)
	bool bIsMonkeyLegs;

	UPROPERTY(BlueprintReadWrite)
	bool bHasJaguarUltimate;
	UPROPERTY(BlueprintReadWrite)
	bool bHasFrogUltimate;
	UPROPERTY(BlueprintReadWrite)
	bool bHasMonkeyUltimate;

	// Bools for items, will be changed to something better
	UPROPERTY(BlueprintReadWrite)
	bool bHasOilSpill;
	UPROPERTY(BlueprintReadWrite)
	bool bHasDroneStrike;
	UPROPERTY(BlueprintReadWrite)
	bool bHasElectricShock;
	UPROPERTY(BlueprintReadWrite)
	bool bHasFakeItemBox;
	UPROPERTY(BlueprintReadWrite)
	bool bHasEnergyBlaster;
	UPROPERTY(BlueprintReadWrite)
	bool bHasRobotRatStealer;

	UPROPERTY(BlueprintReadWrite)
	EItemTypes CurrentItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasAbilitiesLoaded;
	


	UPROPERTY(VisibleAnywhere, Replicated)
	bool bIsHost;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = RacerValues, meta = (AllowPrivateAccess = "true"))
	int CurrentCheckpoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = RacerValues, meta = (AllowPrivateAccess = "true"))
	float RacerHealth;

	float MaxHealth;
	
	/* Max Character Speed */
	float MaxSpeed;

	/* Current Character Speed*/
	UPROPERTY(VisibleAnywhere, Replicated)
	float CurrentSpeed;

	UPROPERTY(VisibleAnywhere, Replicated)
	float SlowSpeed;

	/* Is Shield On? */
	UPROPERTY(VisibleAnywhere, Replicated)
	float ShieldTimer;

	UPROPERTY(VisibleAnywhere, Replicated)
	float SlowTimer;

	UPROPERTY(VisibleAnywhere, Replicated)
	float ItemTimer;

	UPROPERTY(VisibleAnywhere, Replicated)
	int FrogJumps;

	UPROPERTY(VisibleAnywhere, Replicated)
	int MonkeyJumps;

	UPROPERTY(VisibleAnywhere, Replicated)
	float JumpTimeLimit = 0.0f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_ShieldActive)
	bool bIsShieldActive;

	/* Is Attacking? */
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	bool bIsSlowed;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	bool bHighJumpActive;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	bool bHasFastRecovery;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	float AttackFieldTimer;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	float HeadCooldown;
	
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	float ChestCooldown;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	float LegCooldown;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	float UltimateCooldown;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	bool bUsedBulldozer;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Racer|Ability")
	bool bBulldozerGrowthComplete;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Racer|Ability")
	bool bIsAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Racer|Drifting")
	bool bHasSpeedBoost = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Racer|Drifting")
	float SpeedBoostTimer = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Racer|Drifting")
	float SpeedBuildUp = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float PlayerScalingAmount = 1.0f;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	int RacerID;

	/* RepNotify that is called on all clients when value changes*/
	UFUNCTION()
	void OnRep_CurrentSpeed();

	UFUNCTION()
	void OnRep_CharacterRacePosition();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRacerPlaceText(int NewPosition);
	
protected:
	UFUNCTION()
	void OnRep_ShieldActive(bool ShieldStatus);

	/* Handle Updating UI when item is picked up */
	UFUNCTION(BlueprintImplementableEvent)
	void Handle_ItemUI_Update(EItemTypes CurrentItemType);

	void HandleUsingItem();

	UFUNCTION()
	void ResetAllItemTags();

	UFUNCTION()
	void Stun(float TimeToStun = 1.5f);
	
	/* Handle all item funcionality */

	UFUNCTION(Server, Reliable)
	void Server_OilSpillItem();
	void Server_OilSpillItem_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OilSpillItem();
	void Multicast_OilSpillItem_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_DroneStrikeItem();
	void Server_DroneStrikeItem_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DroneStrikeItem();
	void Multicast_DroneStrikeItem_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_ElectricShockItem();
	void Server_ElectricShockItem_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ElectricShockItem();
	void Multicast_ElectricShockItem_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_FakeItemBoxItem();
	void Server_FakeItemBoxItem_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FakeItemBoxItem();
	void Multicast_FakeItemBoxItem_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_EnergyBlasterItem();
	void Server_EnergyBlasterItem_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EnergyBlasterItem();
	void Multicast_EnergyBlasterItem_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_RobotRatItem();
	void Server_RobotRatItem_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RobotRatItem();
	void Multicast_RobotRatItem_Implementation();

	/* Update Speed on server */
	UFUNCTION(Server, Reliable)
	void Server_UpdateOtherRacerSpeed(float newSpeed, ABaseCharacter* OtherRacer, bool bSlowed, float SlowTime);
	void Server_UpdateOtherRacerSpeed_Implementation(float newSpeed, ABaseCharacter* OtherRacer, bool bSlowed, float SlowTime);

	/* Update Speed everywhere */
	UFUNCTION(NetMulticast, Reliable)
	void Multi_UpdateOtherRacerSpeed(float newSpeed, ABaseCharacter* OtherRacer, bool bSlowed, float SlowTime);
	void Multi_UpdateOtherRacerSpeed_Implementation(float newSpeed, ABaseCharacter* OtherRacer, bool bSlowed, float SlowTime);

	UFUNCTION()
	void AttachItem(EItemTypes Item);

	UFUNCTION()
	void OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/* Handle Movement */
	void Move(const FInputActionValue& Value);
	
	/* Handle Turning Character */
	void TurnPlayer(const FInputActionValue& Value);

	/* Handle Moving Camerae */
	void CameraLook(const FInputActionValue& Value);

	/* Handle Moving Camerae */
	void HandleBrakeTrigger(const FInputActionValue& Value);

	/* Handle Moving Camerae */
	void HandleBrakeRelease(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_UpdateCharacterFriction(float NewFriction, bool bIsLetGo);
	
	void Server_UpdateCharacterFriction_Implementation(float NewFriction, bool bIsLetGo);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateCharacterFriction(float NewFriction, bool bIsLetGo);
	
	void Multicast_UpdateCharacterFriction_Implementation(float NewFriction, bool bIsLetGo);

	UFUNCTION(Server, Reliable)
	void Server_UpdateCharacterSpeed(float NewSpeed, float NewAcceleration);
	
	void Server_UpdateCharacterSpeed_Implementation(float NewSpeed, float NewAcceleration);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateCharacterSpeed(float NewSpeed, float NewAcceleration);
	
	void Multicast_UpdateCharacterSpeed_Implementation(float NewSpeed, float NewAcceleration);

	UFUNCTION(Server, Reliable)
	void Server_UpdateCharacterSpeedTemp(float NewSpeed, float NewAcceleration);
	
	void Server_UpdateCharacterSpeedTemp_Implementation(float NewSpeed, float NewAcceleration);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateCharacterSpeedTemp(float NewSpeed, float NewAcceleration);
	
	void Multicast_UpdateCharacterSpeedTemp_Implementation(float NewSpeed, float NewAcceleration);

	UFUNCTION(Server, Reliable)
	void Server_UpdateCharacterSpeedLines(bool bTurnOn);
	
	void Server_UpdateCharacterSpeedLines_Implementation(bool bTurnOn);
	
	UFUNCTION(Client, Reliable)
	void Client_UpdateCharacterSpeedLines(bool bTurnOn);
	
	void Client_UpdateCharacterSpeedLines_Implementation(bool bTurnOn);
	

	/*

	/* Test Abilities
	 * TODO: Convert all this into Ability System at some point
	 */

	// Ability Hold
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleCrosshair();

	// General Ability Functions
	void HandleHeadAbility();
	void HandleBodyAbility();
	void HandleLegAbility();
	void HandleUltimateAbility();

	UFUNCTION(BlueprintCallable)
	void ResetAllAbilityTags(int ID);

	// Monkey Abilities
	void ThrowBanana(FVector Direction, FVector LineOffset, bool bIsUltimate);

	// Passive? Reduce this character's recovery time
	UFUNCTION(Server, Reliable)
	void Server_FastRecovery();
	void Server_FastRecovery_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FastRecovery();
	void Multicast_FastRecovery_Implementation();

	// Throw some objects forward
	UFUNCTION(Server, Reliable)
	void Server_FireTraps();
	void Server_FireTraps_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireTraps();
	void Multicast_FireTraps_Implementation();

	// Jump onto another player
	UFUNCTION(Server, Reliable)
	void Server_MonkeyPounce();
	void Server_MonkeyPounce_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MonkeyPounce();
	void Multicast_MonkeyPounce_Implementation();

	// Monkey Ultimate
	UFUNCTION(Server, Reliable)
	void Server_MonkeyUltimate();
	void Server_MonkeyUltimate_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MonkeyUltimate();
	void Multicast_MonkeyUltimate_Implementation();
	

	// Frog Abilities

	// Grab a player in front of the player and pull them towards you
	UFUNCTION(Server, Reliable)
	void Server_GrappleTongue();
	void Server_GrappleTongue_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GrappleTongue();
	void Multicast_GrappleTongue_Implementation();

	// Activate a shield that damages any other player who touches it
	UFUNCTION(Server, Reliable)
	void Server_PoisionTouch();
	void Server_PoisionTouch_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PoisionTouch();
	void Multicast_PoisionTouch_Implementation();

	// Alter jump to be more like a frog and get three bursts with the new jump
	UFUNCTION(Server, Reliable)
	void Server_MegaLeap();
	void Server_MegaLeap_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MegaLeap();
	void Multicast_MegaLeap_Implementation();

	// Frog Ultimate
	UFUNCTION(Server, Reliable)
	void Server_FrogUltimate();
	void Server_FrogUltimate_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FrogUltimate();
	void Multicast_FrogUltimate_Implementation();

	// Jaguar Abilities

	// Damage anyone within a certain range in front of the player
	UFUNCTION(Server, Reliable)
	void Server_Bite();
	void Server_Bite_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Bite();
	void Multicast_Bite_Implementation();

	// Gain some movement speed and alter shader for a certain amount of time
	UFUNCTION(Server, Reliable)
	void Server_Invisible();
	void Server_Invisible_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Invisible();
	void Multicast_Invisible_Implementation();

	// Increase size for a certain amount of time
	UFUNCTION(Server, Reliable)
	void Server_Bulldozer();
	void Server_Bulldozer_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Bulldozer();
	void Multicast_Bulldozer_Implementation();

	// Jaguar Ultimate
	UFUNCTION(Server, Reliable)
	void Server_JaguarUltimate();
	void Server_JaguarUltimate_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_JaguarUltimate();
	void Multicast_JaguarUltimate_Implementation();
	
	// Play montages 
	UFUNCTION(Server, Reliable)
	void Server_PlayMontage(UAnimMontage* MontageToPlay, float PlayRate = 1.0f);
	void Server_PlayMontage_Implementation(UAnimMontage* MontageToPlay, float PlayRate = 1.0f);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(UAnimMontage* MontageToPlay, float PlayRate = 1.0f);
	void Multicast_PlayMontage_Implementation(UAnimMontage* MontageToPlay, float PlayRate = 1.0f);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Set IsSlowed */
	void SetIsSlowed(bool newSlow);

	/* void SetSlowTimer */
	void SetSlowTimer(float newSlowTimer);

	void SetCurID(int ID);

	int GetCurID();

};
