// Fill out your copyright notice in the Description page of Project Settings.


#include "MysteryBox.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AMysteryBox::AMysteryBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMysteryBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AMysteryBox, CurrentItemType);
}

void AMysteryBox::SetIsFake(bool NewBool)
{
	bIsFake = NewBool;
	if(bIsFake)
	{
		CurrentItemType = EItemTypes::FakeBox;
	}
}

// Called when the game starts or when spawned
void AMysteryBox::BeginPlay()
{
	Super::BeginPlay();

	RandomizeCurrentItem();
}

void AMysteryBox::RandomizeCurrentItem()
{
	int random_int = FMath::RandRange(1, 6);
	switch (random_int)
	{
		case 1:
			CurrentItemType = EItemTypes::OilSpill;
		break;
		case 2:
			CurrentItemType = EItemTypes::DroneStrike;
		break;
		case 3:
			CurrentItemType = EItemTypes::ElectricShock;
		break;
		case 4:
			CurrentItemType = EItemTypes::FakeItemBox;
		break;
		case 5:
			CurrentItemType = EItemTypes::EnergyBlaster;
		break;
		case 6:
			CurrentItemType = EItemTypes::RobotRatStealer;
		break;
	}
}

// Called every frame
void AMysteryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const EItemTypes AMysteryBox::GetItemType()
{
	return CurrentItemType;
}

void AMysteryBox::BeDestroyed()
{
	Destroy();
}

