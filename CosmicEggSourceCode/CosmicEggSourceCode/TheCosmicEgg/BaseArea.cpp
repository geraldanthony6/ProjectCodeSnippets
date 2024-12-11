// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseArea.h"

#include "AICitizen.h"
#include "BaseCitizen.h"

#pragma region "Constructor/Begin/Tick/Getters/Setters/Initalizers"
// Sets default values
ABaseArea::ABaseArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AreaTriggerField = CreateDefaultSubobject<UBoxComponent>("AreaTriggerField");
	AreaTriggerField->SetupAttachment(RootComponent);
	AreaTriggerField->SetBoxExtent(AreaSize);

	AreaTriggerField->OnComponentBeginOverlap.AddDynamic(this, &ABaseArea::OnAreaBeginOverlap);
	AreaTriggerField->OnComponentEndOverlap.AddDynamic(this, &ABaseArea::OnAreaEndOverlap);

}

// Called when the game starts or when spawned
void ABaseArea::BeginPlay()
{
	Super::BeginPlay();

	InitStoreActors();
}

// Called every frame
void ABaseArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<TObjectPtr<AActor>> ABaseArea::GetObjectsInArea()
{
	return ObjectsInArea;
}

TArray<TObjectPtr<AActor>> ABaseArea::GetCharactersInArea()
{
	return CharactersInArea;
}

void ABaseArea::InitStoreActors()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetActorForwardVector() * 500.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionShape BoxShape = FCollisionShape::MakeBox(FVector(500.0f, 500.0f, 300.0f));

	TArray<FHitResult> OutHits;

	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_WorldStatic,
		BoxShape,
		QueryParams);

	if(bHit)
	{
		for(auto Hit : OutHits)
		{
			if(Hit.GetActor()->ActorHasTag("Test") && !ObjectsInArea.Contains(Hit.GetActor()))
			{
				ObjectsInArea.Add(Hit.GetActor());
			}
		}
	}
}
#pragma endregion

#pragma region "Trigger Fields"
void ABaseArea::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Store any object that enters the area in the area actor array list
	if(!CharactersInArea.Contains(OtherActor))
	{
		CharactersInArea.Add(OtherActor);
		// Check if the actor that entered is a player character
		if(TObjectPtr<ABaseCitizen> CurrentBaseCitizen = Cast<ABaseCitizen>(OtherActor))
		{
			CurrentBaseCitizen->SetCurrentArea(this);
		}

		if(TObjectPtr<AAICitizen> CurrentAICitizen = Cast<AAICitizen>(OtherActor))
		{
			CurrentAICitizen->SetCurrentArea(this);
		}
	}
}

void ABaseArea::OnAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the area contains the actor that is leaving
	if(CharactersInArea.Contains(OtherActor))
	{
		// Remove that actor and set its current area to null
		CharactersInArea.Remove(OtherActor);
		if(TObjectPtr<ABaseCitizen> CurrentBaseCitizen = Cast<ABaseCitizen>(OtherActor))
		{
			CurrentBaseCitizen->SetCurrentArea(nullptr);
		}

		if(TObjectPtr<AAICitizen> CurrentAICitizen = Cast<AAICitizen>(OtherActor))
		{
			CurrentAICitizen->SetCurrentArea(nullptr);
		}
	}
}
#pragma endregion 


