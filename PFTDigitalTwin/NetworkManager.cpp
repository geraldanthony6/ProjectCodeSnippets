// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkManager.h"

#include "PFTDigitalTwin.h"
#include "BPFunctionLibs/HttpJasonHandler.h"
#include "UnrealCore/PFTPlayerController.h"

// Sets default values
ANetworkManager::ANetworkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach the component
	HTTPHudRequests = CreateDefaultSubobject<UHTTPRequests>(TEXT("HTTP Component"));
	HTTPRoomInfoRequests = CreateDefaultSubobject<UHTTPRequests>(TEXT("HTTP Requests"));

}

void ANetworkManager::ReceiveHUDRoomData()
{
	//UE_LOG(PFTDigitalTwin, Warning, TEXT("Recieved message from server"));	
	if(APFTPlayerController* CurrentController = Cast<APFTPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		FHUDNetworkData TempHUDNetworkData;
		UHttpJasonHandler::GetHUDRoomFromJson(HTTPHudRequests->ResponseContent, HTTPHudRequests, TempHUDNetworkData);
		//UE_LOG(PFTDigitalTwin, Warning, TEXT("Floor: %d"), TempHUDNetworkData.RoomFloor);
		CurrentController->SetSectorHud(CurrentTrigger, TempHUDNetworkData.RoomName, TempHUDNetworkData.RoomNumber, TempHUDNetworkData.RoomFloor);
		CurrentTrigger = nullptr;	
	}
}

void ANetworkManager::ReceiveRoomInfoData()
{
	UE_LOG(PFTDigitalTwin, Display, TEXT("Room Info: %s"), *HTTPRoomInfoRequests->ResponseContent);
	if(APFTPlayerController* CurrentController = Cast<APFTPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		FRoomInfoNetworkData RoomInfoNetworkData;
		UHttpJasonHandler::GetRoomInfoFromJson(HTTPRoomInfoRequests->ResponseContent, HTTPHudRequests, RoomInfoNetworkData);
		CurrentController->PFTMainMenu->UpdateRoomInfo(RoomInfoNetworkData);
	}
	
}

void ANetworkManager::GetHUDRoomData(const FString& GUID, APFTAreaTriggers* PFTAreaTrigger)
{
	// Keeps track of which area the player is in stored in the player controller
	CurrentTrigger = PFTAreaTrigger;
//	UE_LOG(PFTDigitalTwin, Warning, TEXT("Get HUD Room Data"));
	//MakeGetRequest()
	if (HTTPHudRequests)
	{
	//	UE_LOG(PFTDigitalTwin, Warning, TEXT("URL: %s"), **UrlRequest);
		HTTPHudRequests->MakeGetRequest((HTTPHudRequests->URLRequest) + "/" + GUID);
	}
	else
	{
		UE_LOG(PFTDigitalTwin, Warning, TEXT("No Hud network component in network manager"));
	}
}

void ANetworkManager::GetRoomInfoData(const FString& GUID)
{
	if (HTTPRoomInfoRequests)
	{
		HTTPRoomInfoRequests->MakeGetRequest((HTTPRoomInfoRequests->URLRequest) + "/" + GUID);
	}
	else
	{
		UE_LOG(PFTDigitalTwin, Warning, TEXT("No Room Info network component in network manager"));
	}
}

// Called when the game starts or when spawned
void ANetworkManager::BeginPlay()
{
	Super::BeginPlay();

	HTTPHudRequests->HttpResponse.AddDynamic(this, &ANetworkManager::ReceiveHUDRoomData);
	HTTPRoomInfoRequests->HttpResponse.AddDynamic(this, &ANetworkManager::ReceiveRoomInfoData);
}

// Called every frame
void ANetworkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANetworkManager::MakeGetRequest(FString& URLRequest)
{
	HTTPHudRequests->MakeGetRequest(URLRequest);
}

