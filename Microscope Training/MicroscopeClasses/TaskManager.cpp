// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskManager.h"

#pragma region "Constructor/Tick/Begin"
// Sets default values
ATaskManager::ATaskManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATaskManager::BeginPlay()
{
	Super::BeginPlay();
	
	SetupTaskManager(CurrentTaskManagerData);
}

// Called every frame
void ATaskManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion


#pragma region "Task Handlers"
void ATaskManager::SetupTaskManager(FTaskManagerData NewTaskManagerData)
{
	//CurrentTaskManagerData = NewTaskManagerData;

	switch (CurrentTaskManagerData.CurrentModuleDifficulty)
	{
		case EModuleDifficulty::Beginner:
			StartNewBeginnerTask(CurrentTaskManagerData.CurrentModuleType, 0);
		break;
		case EModuleDifficulty::Intermediate:
			StartNewIntermediateTask(CurrentTaskManagerData.CurrentModuleType, 0);
		break;
		case EModuleDifficulty::Hard:
			StartNewHardTask(CurrentTaskManagerData.CurrentModuleType, 0);
		 break;
		default:
		break;
	}
}

void ATaskManager::StartNewBeginnerTask(ETrainingModuleType CurrentModule, int NewTaskNumber)
{
	switch (CurrentModule)
	{
		case ETrainingModuleType::XRay:
			if(NewTaskNumber < ListOfBeginnerXRayTasks.Num())
			{
				if(bIsFirstTask)
				{
					MaxTaskNumber = ListOfBeginnerXRayTasks.Num();
					bIsFirstTask = false;
				}
				CurrentTaskBeingDone = ListOfBeginnerXRayTasks[NewTaskNumber];
				UpdateTaskManagerUI();
			}
		break;
		case ETrainingModuleType::SpeakWithPatient:
			CurrentTaskBeingDone = ListOfBeginnerPatientTasks[NewTaskNumber];
		break;
		default:
		break;	
	}
}

void ATaskManager::StartNewIntermediateTask(ETrainingModuleType CurrentModule, int NewTaskNumber)
{
	switch (CurrentModule)
	{
		case ETrainingModuleType::XRay:
			CurrentTaskBeingDone = ListOfIntermediateXRayTasks[NewTaskNumber];
		break;
		case ETrainingModuleType::SpeakWithPatient:
			CurrentTaskBeingDone = ListOfIntermediatePatientTasks[NewTaskNumber];
		break;
		default:
		break;	
	}
}

void ATaskManager::StartNewHardTask(ETrainingModuleType CurrentModule, int NewTaskNumber)
{
	switch (CurrentModule)
	{
		case ETrainingModuleType::XRay:
			CurrentTaskBeingDone = ListOfHardXRayTasks[NewTaskNumber];
		break;
		case ETrainingModuleType::SpeakWithPatient:
			CurrentTaskBeingDone = ListOfHardPatientTasks[NewTaskNumber];
		break;
		default:
		break;	
	}
}

void ATaskManager::CompleteCurrentTask()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("Complete Current Task"));
	CurrentTaskNumber++;
	switch (CurrentTaskManagerData.CurrentModuleDifficulty)
	{
		case EModuleDifficulty::Beginner:
			StartNewBeginnerTask(CurrentTaskManagerData.CurrentModuleType, CurrentTaskNumber);
		break;
		case EModuleDifficulty::Intermediate:
			StartNewIntermediateTask(CurrentTaskManagerData.CurrentModuleType, CurrentTaskNumber);
		break;
		case EModuleDifficulty::Hard:
			StartNewHardTask(CurrentTaskManagerData.CurrentModuleType, CurrentTaskNumber);
		break;
		default:
		break;	
	}
}
#pragma endregion 

