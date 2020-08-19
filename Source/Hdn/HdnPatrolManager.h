// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HdnPatrolManager.generated.h"

class AHdnPatrol;
UCLASS()
class HDN_API AHdnPatrolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHdnPatrolManager();
	void RegisterPatrol(AHdnPatrol* patrol);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	TArray<AHdnPatrol*> Patrols;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
