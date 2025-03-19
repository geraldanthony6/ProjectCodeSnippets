// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MysteryBox.generated.h"


UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	OilSpill = 1 UMETA(DisplayName = "OilSpill"),
	DroneStrike = 2 UMETA(DisplayName = "DroneStrike"),
	ElectricShock = 3 UMETA(DisplayName = "ElectricShock"),
	FakeItemBox = 4 UMETA(DisplayName = "FakeItemBox"),
	EnergyBlaster = 5 UMETA(DisplayName = "EnergyBlaster"),
	RobotRatStealer = 6 UMETA(DisplayName = "RobotRatStealer"),
	FakeBox = 7 UMETA(DisplayName = "FakeBox"),
};

UCLASS()
class VEROCITY_API AMysteryBox : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMysteryBox();

	/* Property Replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetIsFake(bool NewBool);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Item, meta = (AllowPrivateAccess = "true"))
	EItemTypes CurrentItemType;

	bool bIsFake;

	// Randomizes the item assigned to an instance of a mystery box
	UFUNCTION()
	void RandomizeCurrentItem();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const EItemTypes GetItemType();

	void BeDestroyed();

};
