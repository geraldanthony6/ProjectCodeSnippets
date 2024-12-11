// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractableObject.h"
#include "GameFramework/Actor.h"
#include "TaskManager.generated.h"

// Struct that defines which type of training module is active
UENUM(BlueprintType)
enum class ETrainingModuleType : uint8
{
	NONE,
	XRay,
	SpeakWithPatient,
};

// Struct that defines the difficulty of the active module
UENUM(BlueprintType)
enum class EModuleDifficulty : uint8
{
	NONE,
	Beginner,
	Intermediate,
	Hard
};

// Struct that holds the module type and difficulty
USTRUCT(BlueprintType)
struct FTaskManagerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ETrainingModuleType CurrentModuleType;

	UPROPERTY(EditAnywhere)
	EModuleDifficulty CurrentModuleDifficulty;
	
};

// Struct that holds the current active task and its associated components
USTRUCT(BlueprintType)
struct FTaskData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ABaseInteractableObject> InteractableObject;

	UPROPERTY(EditAnywhere)
	ETrainingModuleType CurrentTaskType;

	UPROPERTY(EditAnywhere)
	int TaskNumber;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TaskDescription;
};

UCLASS()
class DMAEVRPLUGIN_API ATaskManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskManager();

	// Sets up the Current Tasks Manager Parts struct
	UFUNCTION(BlueprintCallable)
	void SetupTaskManager(FTaskManagerData NewTaskManagerData);
	
	// Begins a new task based on difficulty and module type
	void StartNewBeginnerTask(ETrainingModuleType CurrentModule, int NewTaskNumber);
	void StartNewIntermediateTask(ETrainingModuleType CurrentModule, int NewTaskNumber);
	void StartNewHardTask(ETrainingModuleType CurrentModule, int NewTaskNumber);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateTaskManagerUI();

	// Handles completing a task and starting a new one
	UFUNCTION(BlueprintCallable)
	void CompleteCurrentTask();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTaskManagerData CurrentTaskManagerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTaskData CurrentTaskBeingDone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskData> ListOfBeginnerXRayTasks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskData> ListOfIntermediateXRayTasks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskData> ListOfHardXRayTasks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskData> ListOfBeginnerPatientTasks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskData> ListOfIntermediatePatientTasks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskData> ListOfHardPatientTasks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentTaskNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFirstTask = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxTaskNumber;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
