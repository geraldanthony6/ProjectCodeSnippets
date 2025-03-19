// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractableObject.h"
#include "TaskManager.h"

#pragma region "Constructor/Begin/Tick/Setters"
// Sets default values
ABaseInteractableObject::ABaseInteractableObject()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void ABaseInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseInteractableObject::SetCurrentInteractablePieceBasedOnMesh(UStaticMeshComponent* NewMesh)
{
	for (FInteractablePiece Piece : InteractablePieces)
	{
		if(Piece.InteractablePieceMesh == NewMesh)
		{
			CurrentHoveredPiece = Piece;
		}
	}
}
#pragma endregion 

#pragma region "Object and Piece Interaction"
void ABaseInteractableObject::AddInteractablePieceToArray(FInteractablePiece NewPiece)
{
	InteractablePieces.Add(NewPiece);
	NewPiece.InteractablePieceMesh->OnClicked.AddDynamic(this, &ABaseInteractableObject::OnInteractablePieceClicked);
	NewPiece.InteractablePieceMesh->SetGenerateOverlapEvents(true);
	NewPiece.InteractablePieceMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractableObject::OnInteractablePieceBeginOverlap);
}

void ABaseInteractableObject::OnInteractablePieceClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	// Set the ui object to move to this piece's desired location and set the ui text to this piece's text. (Use a blueprint function)
	if(!bIsTaskCompleted)
	{
		UpdateTaskUI(InteractableTaskNumber);
		TaskManager->CompleteCurrentTask();
		bIsTaskCompleted = true;
	}

}

void ABaseInteractableObject::OnInteractablePieceBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (FInteractablePiece TempPiece : InteractablePieces)
	{
		if(Cast<UStaticMeshComponent>(OverlappedComp) == TempPiece.InteractablePieceMesh)
		{
			CurrentHoveredPiece = TempPiece;
			//sGEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, CurrentHoveredPiece.InfoToDisplay.ToString());
		}
	}
}

void ABaseInteractableObject::OnInteractablePieceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (FInteractablePiece TempPiece : InteractablePieces)
	{
		if(Cast<UStaticMeshComponent>(OverlappedComp) == TempPiece.InteractablePieceMesh)
		{
			//CurrentHoveredPiece = nullptr;
		}
	}
}
#pragma endregion 

