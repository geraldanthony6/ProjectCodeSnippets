// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputCoreTypes.h"
#include "BaseInteractableObject.generated.h"

// Struct that holds the information related to an interactable piece of equipment
USTRUCT(BlueprintType)
struct FInteractablePiece
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> InteractablePieceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocationOfInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotationOfDisplay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InfoToDisplay;
	
};

UCLASS()
class DMAEVRPLUGIN_API ABaseInteractableObject : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseInteractableObject();

	// Current piece of the interactable object being hovered over by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FInteractablePiece CurrentHoveredPiece;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Array of all interactable pieces attached to this interactable object
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInteractablePiece> InteractablePieces;

	// The task manager this interactable object is connected to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class ATaskManager> TaskManager;

	// The task number associated with this object
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InteractableTaskNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsTaskCompleted = false;

	// Description of the task being completed on this particular object
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TaskDescription;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void AddInteractablePieceToArray(FInteractablePiece NewPiece);

	// Handles what occurs when an interactable piece is clicked
	UFUNCTION()
	void OnInteractablePieceClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	// Update the ui that is attached to this interactable object
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateObjectUI(FInteractablePiece CurPiece);

	// Set the task manager ui to match the task associated with this object
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTaskUI(int TaskNumber);

	// Update the task manager ui once this task is completed
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateTaskUI(int TaskNumber);

	// Handles the conditions that must be met for this task to be completed
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HandleTask(int TaskNumber);

	UFUNCTION(BlueprintCallable)
	void SetCurrentInteractablePieceBasedOnMesh(UStaticMeshComponent* NewMesh);


protected:
	
	UFUNCTION()
	void OnInteractablePieceBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractablePieceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	

};
