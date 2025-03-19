// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigitalTwinTools/Public/Components/HTTPRequests.h"
#include "Volumes/PFTAreaTriggers.h"
#include "NetworkManager.generated.h"

//Struct to HUD Network data from database
USTRUCT(BlueprintType)
struct FHUDNetworkData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	int32 RoomFloor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	FString RoomName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	FString RoomNumber;
};

//Struct to store all network data from database
USTRUCT(BlueprintType)
struct FRoomInfoNetworkData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	int32 RoomFloor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	FString RoomName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	FString RoomNumber;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	int32 RoomSqFootage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	FString Department;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	TArray<FString> RoomOwners;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Networking")
	FString RoomType;
};


UCLASS()
class PFTDIGITALTWIN_API ANetworkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkManager();

	UFUNCTION()
	void ReceiveHUDRoomData();

	UFUNCTION()
	void ReceiveRoomInfoData();

	UFUNCTION()
	void GetHUDRoomData(const FString& GUID, APFTAreaTriggers* PFTAreaTrigger);

	UFUNCTION()
	void GetRoomInfoData(const FString& GUID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Declare the component
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Networking")
    UHTTPRequests* HTTPHudRequests;

	// Declare the component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Networking")
	UHTTPRequests* HTTPRoomInfoRequests;

	UPROPERTY()
	APFTAreaTriggers* CurrentTrigger;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MakeGetRequest(FString& URLRequest);

};
