// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HdnEscapeObjective.generated.h"

class UBoxComponent;
UCLASS()
class HDN_API AHdnEscapeObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHdnEscapeObjective();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UBoxComponent* InteractZone;
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
