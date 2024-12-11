// Fill out your copyright notice in the Description page of Project Settings.


#include "AICitizen.h"

#pragma region "Constructor/Begin/Tick/Getters/Setters"
// Sets default values
AAICitizen::AAICitizen()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAICitizen::BeginPlay()
{
	Super::BeginPlay();

	RandomlySwitchEnum();
}

// Called every frame
void AAICitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICitizen::SetCurrentArea(TObjectPtr<ABaseArea> NewArea)
{
	CurrentArea = NewArea;
	UpdateBehaviorTreeWithNewArea();
}

TArray<FActionStruct> AAICitizen::GetPossibleActionsArray()
{
	return PossibleActionsArray;
}

#pragma endregion

#pragma region "Trigger Fields"
void AAICitizen::OnActionTriggerFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AAICitizen::OnActionTriggerFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

#pragma endregion


#pragma region "Choosing and Performing Action Logic"
void AAICitizen::ChooseAction()
{
	switch (CurrentCharacterImpulse)
	{
		case ECharacterImpulse::WALK:
			WalkAroundArea();
		break;
		case ECharacterImpulse::INTERACTOBJECT:
			InteractWithObject();
		break;
		case ECharacterImpulse::INTERACTCHARACTER:
			InteractWithCharacter();
		break;
		case ECharacterImpulse::EXPLORENEWAREA:
			ExploreNewArea();
		break;
		default:
		break;;
	}

}

void AAICitizen::PerformAction()
{
	switch (CurrentCharacterImpulse)
	{
		case ECharacterImpulse::WALK:
			PerformWalkAroundAction();
			break;
		case ECharacterImpulse::INTERACTOBJECT:
			PerformActionOnObject();
			break;
		case ECharacterImpulse::INTERACTCHARACTER:
			PerformActionOnCharacter();
			break;
		case ECharacterImpulse::EXPLORENEWAREA:
			PerformActionOfExploringNewArea();
			break;
		default:
			break;
	}
}

void AAICitizen::WalkAroundArea()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Walk Around da area");
}

void AAICitizen::InteractWithObject()
{
	// Get current area and check which objects have interactable tag and store them in an array or something
	if(ObjectsInteractedWith.Num() != CurrentArea->GetObjectsInArea().Num())
	{
		TArray<TObjectPtr<AInteractableObjectBase>> ArrayOfPossibleObjects;
		for (auto CurActor : CurrentArea->GetObjectsInArea())
		{
			if(TObjectPtr<AInteractableObjectBase> TempInteractable = Cast<AInteractableObjectBase>(CurActor))
			{
				ArrayOfPossibleObjects.Add(TempInteractable);
			}
		}
		// Here the AI should choose between randomly walking around for a bit, interacting with an interactable object, or interacting with another player.
		// Iterate through the array and randomly select one and store it into current interaction object or something
		for (auto TempInteractable : ArrayOfPossibleObjects)
		{
			// If the interactable has a good number for your tendency store it
			if(TempInteractable->ActorHasTag("Test") && !ObjectsInteractedWith.Contains(TempInteractable))
			{
				CurrentInteractableObject = TempInteractable;
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, CurrentInteractableObject->GetName());
	} else if(ObjectsInteractedWith.Num() == CurrentArea->GetObjectsInArea().Num())
	{
		ObjectsInteractedWith.Empty();
		ChooseAction();
	}
}

void AAICitizen::InteractWithCharacter()
{
	// Get current areas and check if there are characters in the area
	TArray<TObjectPtr<AAICitizen>> ArrayOfPossibleCharacters;
	for(auto CurActor : CurrentArea->GetCharactersInArea())
	{
		if(TObjectPtr<AAICitizen> TempCharacter = Cast<AAICitizen>(CurActor))
		{
			ArrayOfPossibleCharacters.Add(TempCharacter);
		}
	}

	for (auto TempCharacter : ArrayOfPossibleCharacters)
	{
		// If the interactable has a good number for your tendency store it
		if(TempCharacter->ActorHasTag("Test"))
		{
			CurrentCharacterBeingInteractedWith = TempCharacter;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, CurrentCharacterBeingInteractedWith->GetName());
}

void AAICitizen::ExploreNewArea()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Go to new area");
}

void AAICitizen::PerformWalkAroundAction()
{
	RandomlySwitchEnum();
}

void AAICitizen::PerformActionOnObject()
{
	// Iterate through the available actions of the selected objects and do an action
	for (auto TempAction : CurrentInteractableObject->GetPossibleActionsArray())
	{
		if(TempAction.ValueToAddToEnergy > 30.0f)
		{
			CurrentActionToTake = TempAction;
		}
	}
	
	// Do the animation and add the values to the player
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, CurrentActionToTake.ActionName);

	// Reset action and interactable object and find a new one
	ObjectsInteractedWith.Add(CurrentInteractableObject);
	CurrentInteractableObject = nullptr;
	RandomlySwitchEnum();
}

void AAICitizen::PerformActionOnCharacter()
{
	for (auto TempAction : CurrentCharacterBeingInteractedWith->GetPossibleActionsArray())
	{
		if(TempAction.ValueToAddToEnergy > 10.0f)
		{
			CurrentActionToTake = TempAction;
		}
	}

	// Do the animation and add the values to the player
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, CurrentActionToTake.ActionName);

	// Reset action and interactable object and find a new one
	CurrentCharacterBeingInteractedWith = nullptr;
	RandomlySwitchEnum();
}

void AAICitizen::PerformActionOfExploringNewArea()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Go to new area");
	RandomlySwitchEnum();
}

void AAICitizen::RandomlySwitchEnum()
{
	/*int RandomInt = FMath::RandRange(0, 3);
	switch (RandomInt)
	{
		case 0:
			CurrentCharacterImpulse = ECharacterImpulse::WALK;
		break;
		case 1:
			CurrentCharacterImpulse = ECharacterImpulse::INTERACTOBJECT;
		break;
		case 2:
			CurrentCharacterImpulse = ECharacterImpulse::INTERACTCHARACTER;
		break;
		case 3:
			CurrentCharacterImpulse = ECharacterImpulse::EXPLORENEWAREA;
		break;	
		default:
		break;	
	}*/

	CurrentCharacterImpulse = ECharacterImpulse::INTERACTOBJECT;
}

#pragma endregion

