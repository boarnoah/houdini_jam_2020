// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnSpectrumAnalyzer.h"

#include "HdnFlag.h"
#include "HdnGameState.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UHdnSpectrumAnalyzer::UHdnSpectrumAnalyzer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UHdnSpectrumAnalyzer::SetFovSetting(EScannerFov fovSetting)
{
	FovSetting = fovSetting;

	if (FovSetting == EScannerFov::Wide)
	{
		CurrentFovHalf = ScannerFovWide / 2;
	} else
	{
		CurrentFovHalf = ScannerFovNarrow / 2;
	}
}

void UHdnSpectrumAnalyzer::Scan()
{
	auto Objectives = GetWorld()->GetGameState<AHdnGameState>()->Objectives;
	FVector forwardVector = Camera->GetForwardVector();
	forwardVector.Z = 0;
	forwardVector.Normalize();

	VisibleObjectives = TArray<AHdnFlag*>();

	for (AHdnFlag* Objective : Objectives)
	{
		FVector objectivePosition = Objective->GetActorLocation();
		objectivePosition.Z = 0;
		FVector cameraLocation = Camera->GetComponentLocation();
		cameraLocation.Z = 0;
		FVector lookAtVector = objectivePosition - cameraLocation;
		lookAtVector.Normalize();

		const float angle = FMath::Acos(FVector::DotProduct(forwardVector, lookAtVector));

		if (FMath::RadiansToDegrees(angle) <= CurrentFovHalf)
		{
			VisibleObjectives.Add(Objective);
		}
	}
}

// Called when the game starts
void UHdnSpectrumAnalyzer::BeginPlay()
{
	Super::BeginPlay();
	Camera = Cast<USceneComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
}


// Called every frame
void UHdnSpectrumAnalyzer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

