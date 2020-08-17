// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EScannerFov.h"
#include "HdnFlag.h"

#include "HdnSpectrumAnalyzer.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HDN_API UHdnSpectrumAnalyzer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHdnSpectrumAnalyzer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	EScannerFov FovSetting = EScannerFov::Wide;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float ScannerFovWide = 90.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float ScannerFovNarrow = 25.0f;

	void SetFovSetting(EScannerFov fovSetting);
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float CurrentFovHalf = ScannerFovWide / 2;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<AHdnFlag*> VisibleObjectives;

	void Scan();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* Camera;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
