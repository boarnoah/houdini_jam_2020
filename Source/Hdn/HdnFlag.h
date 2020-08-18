// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HdnFlag.generated.h"

UCLASS()
class HDN_API AHdnFlag : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay",  meta = (AllowPrivateAccess = "true"))
	bool ObjectiveEnabled = false;

public:	
	// Sets default values for this actor's properties
	AHdnFlag();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UBoxComponent* InteractZone;

	bool GetObjectiveEnabled() const { return ObjectiveEnabled; }
	void SetObjectiveEnabled(bool enabled);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
	bool ObjectiveActivated = false;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
