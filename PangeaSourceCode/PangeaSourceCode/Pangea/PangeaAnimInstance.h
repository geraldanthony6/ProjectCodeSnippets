// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PangeaAnimInstance.generated.h"

UENUM()
enum class ECharacterState : uint8
{
	Locomotion,
	Attack,
	Hit,
	Die
};
/**
 * 
 */
UCLASS()
class PANGEA_API UPangeaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PangeaAnimInstance Params")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PangeaAnimInstance Params")
	ECharacterState State;

	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();
	
};
