// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EEnemyState.h"
#include "GameFramework/Character.h"

#include "HdnEGunship.generated.h"

class AHdnPatrol;
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

	UFUNCTION()
	void OnPlayerSeen(APawn* Pawn);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float AlertedTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float AlertGracePeriod = 3.0f;
	bool InAlertGracePeriod = false;

	FTimerHandle AlertGraceHandle;
	void OnAlertGracePeriodExpired();

	// Player total alert duration
	FTimerHandle AlertHandle;
	void OnAlertExpired();

	void LookAtAlert();
	void InvestigateAlert();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	EEnemyState State = EEnemyState::Patrol;

	UPROPERTY()
	FVector LastSeenPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float WeaponCooldown = 5.0f;

	FTimerHandle WeaponCooldownHandle;
	void OnWeaponCoolDown();
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float CombatCooldown = 10.0f;

	FTimerHandle CombatCooldownHandle;
	void OnCombatCooldown();

	UPROPERTY()
	int CurrentWaypointIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float WaypointReachDistance = 100.0f;

	UPROPERTY()
	FVector CurrentDestination;

	void SetNextWaypoint();

	void ResumePatrol();

	FVector GetPointOnNavMesh(FVector Point) const;
public:
	UPROPERTY()
	AHdnPatrol* Patrol;

	// Called every frame
	virtual void Tick(float DeltaTime) override;  

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
