// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseArea.h"
#include "InteractableObjectBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "AICitizen.generated.h"

UENUM(Blueprintable)
enum class ECharacterImpulse : uint8
{
	NONE,
	WALK,
	INTERACTOBJECT,
	INTERACTCHARACTER,
	EXPLORENEWAREA,
};

UCLASS()
class THECOSMICEGG_API AAICitizen : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICitizen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _NeutralActionTendency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _LucifericActionTendency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _AhrimanicActionTendency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ChristActionTendency;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasReachedCurrentObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ABaseArea> CurrentArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AInteractableObjectBase> CurrentInteractableObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AInteractableObjectBase>> ObjectsInteractedWith;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AAICitizen> CurrentCharacterBeingInteractedWith;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FActionStruct CurrentActionToTake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterImpulse CurrentCharacterImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActionStruct> PossibleActionsArray;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Set current area this actor is in
	void SetCurrentArea(TObjectPtr<ABaseArea> NewArea);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBehaviorTreeWithNewArea();

	TArray<FActionStruct> GetPossibleActionsArray();


protected:
	// Handle what happens when this actor overlaps another
	UFUNCTION()
	void OnActionTriggerFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Handle what happens when this actor stops overlapping another
	UFUNCTION()
	void OnActionTriggerFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Choose which action to take based on the current character impulse enum
	UFUNCTION(BlueprintCallable)
	void ChooseAction();

	// Perform an action based on the current character impulse enum
	UFUNCTION(BlueprintCallable)
	void PerformAction();

	// Handles when an actor chooses to walk around the current area they are in
	UFUNCTION(BlueprintCallable)
	void WalkAroundArea();

	// Handles when an actor chooses to interact with an object in the current area they are in
	UFUNCTION(BlueprintCallable)
	void InteractWithObject();

	// Handles when an actor chooses to interact with another character that is in the current area they are in
	UFUNCTION(BlueprintCallable)
	void InteractWithCharacter();

	// Handles when an actor chooses to leave the current area and explore a new one
	UFUNCTION(BlueprintCallable)
	void ExploreNewArea();

	// Handles performing the walk action
	UFUNCTION(BlueprintCallable)
	void PerformWalkAroundAction();

	// Handles interacting with an object 
	UFUNCTION(BlueprintCallable)
	void PerformActionOnObject();

	// Handles interacting with another character
	UFUNCTION(BlueprintCallable)
	void PerformActionOnCharacter();

	// Handles finding a new area to explore and moving to that area
	UFUNCTION(BlueprintCallable)
	void PerformActionOfExploringNewArea();

	// Handles switching between different states using a rand int to select different impulse enums
	UFUNCTION(BlueprintCallable)
	void RandomlySwitchEnum();

};
