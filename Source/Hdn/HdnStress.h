// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HdnStress.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDN_API UHdnStress : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHdnStress();

	void AddStress(float amount);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
    float CurrentStress = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Game over when over stressed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float MaxStress = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float HysteresisTime = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	bool Hysteresis = false;

	FTimerHandle HysteresisHandle;

	void HysteresisTimeout();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
