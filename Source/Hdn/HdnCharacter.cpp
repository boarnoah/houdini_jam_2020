// Copyright Epic Games, Inc. All Rights Reserved.

#include "HdnCharacter.h"
#include "HdnFlag.h"


#include "DrawDebugHelpers.h"
#include "HdnEscapeObjective.h"
#include "HdnGameMode.h"
#include "HdnSpectrumAnalyzer.h"
#include "HdnStress.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AHdnCharacter

AHdnCharacter::AHdnCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	SpectrumAnalyzer = CreateDefaultSubobject<UHdnSpectrumAnalyzer>(TEXT("Analyzer"));
	Stress = CreateDefaultSubobject<UHdnStress>(TEXT("Stress"));
}

void AHdnCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerTickHandle, this, &AHdnCharacter::TimerTick, 0.5f, true, -1);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHdnCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHdnCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHdnCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHdnCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHdnCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AHdnCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHdnCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AHdnCharacter::OnResetVR);
}


void AHdnCharacter::ChangeScannerFov() const
{
	if (SpectrumAnalyzer->FovSetting == EScannerFov::Narrow)
	{
		SpectrumAnalyzer->FovSetting = EScannerFov::Wide;
	} else
	{
		SpectrumAnalyzer->FovSetting = EScannerFov::Narrow;
	}
}

// TODO: Allow for recovery from feral with a QTE / Button Mash that gets increasingly harder the more this happens
void AHdnCharacter::ActivateFeral()
{
	auto Gm = Cast<AHdnGameMode>( GetWorld()->GetAuthGameMode());
	Gm->ActivateFeral(this);
}

float AHdnCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float actualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Log, TEXT("Took %f damage, health: %f"), actualDamage, Stress->CurrentStress);
	Stress->AddStress(actualDamage);
	return actualDamage;
}

void AHdnCharacter::TimerTick()
{
	SpectrumAnalyzer->Scan();
	// UE_LOG(LogTemp, Log, TEXT("Num Objectives Visible: %d"), SpectrumAnalyzer->VisibleObjectives.Num());
}

void AHdnCharacter::ActivatedObjective(AHdnFlag* objective) const
{
	auto Gm = Cast<AHdnGameMode>( GetWorld()->GetAuthGameMode());
	Gm->ActivateFlagObjective(objective);
}

void AHdnCharacter::ActivatedEscape(AHdnEscapeObjective* objective) const
{
	auto Gm = Cast<AHdnGameMode>( GetWorld()->GetAuthGameMode());
	Gm->ActivateEscapeObjective(objective);
}

void AHdnCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AHdnCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AHdnCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AHdnCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHdnCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHdnCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHdnCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
