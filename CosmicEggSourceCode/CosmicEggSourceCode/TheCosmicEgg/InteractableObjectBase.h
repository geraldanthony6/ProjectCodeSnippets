// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "InteractableObjectBase.generated.h"

USTRUCT(BlueprintType)
struct FActionStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> AnimMontageForAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValueToAddToEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ActionName;
};

UCLASS()
class THECOSMICEGG_API AInteractableObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> _InteractableTriggerField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _InteractableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UMaterial> _BaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UMaterial> _BeingInteractedWithMaterial;

#pragma region "Amount of energy given"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction | Energy")
	float _NeutralEnergyGiven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction | Energy")
	float _LucifericEnergyGiven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction | Energy")
	float _AhrimanicEnergyGiven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction | Energy")
	float _ChristicEnergyGiven;
#pragma endregion 

	// An array of possible actions that could be taken by a citizen (AI or player) when they interact with this object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction | Actions")
	TArray<FActionStruct> PossibleActionsArray;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FActionStruct> GetPossibleActionsArray();

protected:

	UFUNCTION()
	void OnTriggerFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
