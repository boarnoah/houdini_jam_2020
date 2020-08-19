// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HdnPatrol.generated.h"

class AHdnEScout;
class AHdnEGunship;
class UBoxComponent;
class ATargetPoint;
UCLASS()
class HDN_API AHdnPatrol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHdnPatrol();

	void SetPatrolEnabled(bool status);
	bool GetPatrolEnabled()  const { return Enabled; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	UBoxComponent* SpawnBox;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	bool Enabled = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	int MaxGunships = 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	int MaxScouts = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<AHdnEGunship> GunshipClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<AHdnEScout> ScoutClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	TArray<AHdnEGunship*> Gunships;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	TArray<AHdnEScout*> Scouts;

	void SpawnEnemies();

	static void ToggleEnemyActive(AActor* enemy, bool newState);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
