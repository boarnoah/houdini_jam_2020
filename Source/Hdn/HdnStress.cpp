// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnStress.h"

#include "HdnCharacter.h"

// Sets default values for this component's properties
UHdnStress::UHdnStress()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHdnStress::AddStress(float amount)
{
	if (Hysteresis == false)
	{
		Hysteresis = true;
		GetOwner()->GetWorldTimerManager().SetTimer(HysteresisHandle, this, &UHdnStress::HysteresisTimeout,
		                                            HysteresisTime);

		if (CurrentStress + amount >= MaxStress)
		{
			auto owner = Cast<AHdnCharacter>(GetOwner());
			owner->ActivateFeral();
		}
		else
		{
			CurrentStress += amount;
		}
	} else {
		CurrentStress += amount / 2;
	}
}


// Called when the game starts
void UHdnStress::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UHdnStress::HysteresisTimeout()
{
	Hysteresis = false;
}

// Called every frame
void UHdnStress::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
