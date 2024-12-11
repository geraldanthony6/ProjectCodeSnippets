// Fill out your copyright notice in the Description page of Project Settings.


#include "PangeaAnimInstance.h"

#include "PangeaCharacter.h"

void UPangeaAnimInstance::OnStateAnimationEnds()
{
	if(State == ECharacterState::Attack)
	{
		State = ECharacterState::Locomotion;
	}
	else
	{
		auto character = Cast<APangeaCharacter>(GetOwningActor());
		if(State == ECharacterState::Hit)
		{
			if(character->GetHealthPoints() > 0.0f)
			{
				State = ECharacterState::Locomotion;
			}
			else
			{
				State = ECharacterState::Die;
			}
		}
		else if(State == ECharacterState::Die)
		{
			character->DieProcess();
		}
	}
}
