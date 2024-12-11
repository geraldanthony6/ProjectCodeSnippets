// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefenseTower.generated.h"

UCLASS(Blueprintable)
class PANGEA_API ADefenseTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenseTower();

	UPROPERTY(EditAnywhere, Category="Tower Params")
	int HealthPoints = 100;
	UPROPERTY(EditAnywhere, Category="Tower Params")
	int ShellDefense = 2;
	UPROPERTY(EditAnywhere, Category="Tower Params")
	float AttackRange = 15.0f;
	UPROPERTY(EditAnywhere, Category="Tower Params")
	float ReloadInterval = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int _HealthPoints;
	float _ReloadCountingDown;

	UClass* _FireballClass;

	UPROPERTY(VisibleAnywhere)
	class APlayerAvatar* _Target = nullptr;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* _SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE USphereComponent* GetBoxComponent() const
	{
		return _SphereComponent;
	}

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

	UFUNCTION(BlueprintPure, Category = "Pangea|Defense Tower", meta = (DisplayName = "GetHP"))
	int GetHealthPoints();
	UFUNCTION(BlueprintPure, Category = "Pangea|Defense Tower")
	bool IsDestroyed();
	UFUNCTION(BlueprintPure, Category = "Pangea|Defense Tower")
	bool CanFire();
	void Fire();
	void Hit(int Damage);

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void OnMeshBeginOverlap(AActor* OtherActor);
	
	void DestroyProcess();

};
