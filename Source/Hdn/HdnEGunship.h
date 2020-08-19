// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "HdnEGunship.generated.h"

class UPawnSensingComponent;
UCLASS()
class HDN_API AHdnEGunship : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHdnEGunship();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	UPawnSensingComponent* PawnSensing;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;  

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
