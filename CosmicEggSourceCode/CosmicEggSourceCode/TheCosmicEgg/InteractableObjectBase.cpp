// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjectBase.h"

#include "BaseCitizen.h"

#pragma region "Constructor, Begin Play, Tick"
// Sets default values
AInteractableObjectBase::AInteractableObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_InteractableTriggerField = CreateDefaultSubobject<USphereComponent>("Interactable Trigger Field");
	SetRootComponent(_InteractableTriggerField);
	
	_InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>("Interactable Mesh");
	_InteractableMesh->SetupAttachment(RootComponent);

	_InteractableTriggerField->SetSphereRadius(400.0f);
	_InteractableTriggerField->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObjectBase::OnTriggerFieldBeginOverlap);
	_InteractableTriggerField->OnComponentEndOverlap.AddDynamic(this, &AInteractableObjectBase::OnTriggerFieldEndOverlap);
	
}

// Called when the game starts or when spawned
void AInteractableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion 

#pragma region "Overlap Functions"
void AInteractableObjectBase::OnTriggerFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<ABaseCitizen> CurCitizen = Cast<ABaseCitizen>(OtherActor);
	if(CurCitizen != nullptr)
	{
		CurCitizen->RecieveEnergyFromAction(_NeutralEnergyGiven, _LucifericEnergyGiven, _AhrimanicEnergyGiven, _ChristicEnergyGiven);
		_InteractableMesh->SetMaterial(0, _BeingInteractedWithMaterial);
	}
}

void AInteractableObjectBase::OnTriggerFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<ABaseCitizen> CurCitizen = Cast<ABaseCitizen>(OtherActor);
	if(CurCitizen != nullptr)
	{
		_InteractableMesh->SetMaterial(0, _BaseMaterial);
	}
}
#pragma endregion

TArray<FActionStruct> AInteractableObjectBase::GetPossibleActionsArray()
{
	return PossibleActionsArray;
}



