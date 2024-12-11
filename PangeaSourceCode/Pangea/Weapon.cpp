// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "PlayerAvatar.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_StaticMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Holder == nullptr)
	{
		FQuat rotQuat = FQuat(FRotator(0, 300.0f * DeltaTime, 0));
		AddActorLocalRotation(rotQuat);
	}
	
}

void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	auto character = Cast<APangeaCharacter>(OtherActor);
	if(character == nullptr)
	{
		return;
	}
	
	if(Holder == nullptr)
	{
		auto playerAvatar = Cast<APlayerAvatar>(character);
		if(playerAvatar != nullptr)
		{
			Holder = character;
			TArray<AActor*> attachedActors;
			OtherActor->GetAttachedActors(attachedActors, true);
			for(int i = 0; i < attachedActors.Num(); ++i)
			{
				attachedActors[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				attachedActors[i]->SetActorRotation(FQuat::Identity);
				AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);
				weapon->Holder = nullptr;
			}
			AttachToComponent(Holder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
		}
	}
	else if(IsWithinAttackRange(0.0f, OtherActor))
	{
		// Deal damage to the target
	}
}

bool AWeapon::IsWithinAttackRange(float AttackRange, AActor* Target)
{
	return true;
}



