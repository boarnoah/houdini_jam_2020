// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HdnFlagManager.generated.h"

class AHdnFlag;
UCLASS()
class HDN_API AHdnFlagManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHdnFlagManager();
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	TArray<AHdnFlag*> ObjectiveFlags;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
