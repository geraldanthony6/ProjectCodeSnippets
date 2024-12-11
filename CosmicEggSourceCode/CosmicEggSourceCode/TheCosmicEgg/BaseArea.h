// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BaseArea.generated.h"

UCLASS()
class THECOSMICEGG_API ABaseArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The trigger field that an area has. Anything in this area is attached to this room until it is moved/leaves
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> AreaTriggerField;

	// Store the actors currently in the area
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> ObjectsInArea;

	// Store Characters currently in area
	TArray<TObjectPtr<AActor>> CharactersInArea;

	// Size of the trigger field
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AreaSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	TArray<TObjectPtr<AActor>> GetObjectsInArea();

	TArray<TObjectPtr<AActor>> GetCharactersInArea();

protected:

	// Handle what happens when an actor enters the area field
	UFUNCTION()
	void OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Handle what happens when an actor leaves the area field
	UFUNCTION()
	void OnAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Store the actors in the trigger field of an area at the start of the simulation
	UFUNCTION()
	void InitStoreActors();

};
