// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Death_MAZECharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Inventory_Component.h"
#include "Components/SphereComponent.h"
#include "Items/Item.h"
#include "Character/CharacterParametrs/CharacterParametrs.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"






//////////////////////////////////////////////////////////////////////////
// ADeath_MAZECharacter

ADeath_MAZECharacter::ADeath_MAZECharacter()
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
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

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


	// Create a Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventory_Component>(TEXT("InventoryComponent"));

	CharacterParametersComponent = CreateDefaultSubobject<UCharacterParametrs>(TEXT("CharacterParametrs"));

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollectionSphere->SetupAttachment(GetCapsuleComponent());
	CollectionSphere->SetCollisionProfileName("Custom");
	CollectionSphere->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CollectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	CollectionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	CollectionSphere->SetSphereRadius(150);
	
	
	SetAccelerationAllow(true);
	

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADeath_MAZECharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADeath_MAZECharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADeath_MAZECharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADeath_MAZECharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADeath_MAZECharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADeath_MAZECharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADeath_MAZECharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADeath_MAZECharacter::OnResetVR);

	// Bind key E and function which PickUp items
	PlayerInputComponent->BindAction("E PickUp Item", IE_Pressed, this, &ADeath_MAZECharacter::PickUp_Item);

	PlayerInputComponent->BindAction("Acceleration", IE_Pressed, this, &ADeath_MAZECharacter::Acceleration);
	PlayerInputComponent->BindAction("Acceleration", IE_Released, this, &ADeath_MAZECharacter::AccelerationButton_Released);
	PlayerInputComponent->BindAction("SetWallonTheFloor", IE_Pressed, this, &ADeath_MAZECharacter::SetWallOnTheFloor);



}

UInventory_Component * ADeath_MAZECharacter::GetInventoryComponent()
{

	return InventoryComponent;
}

UCharacterParametrs * ADeath_MAZECharacter::GetCharacterParametersComponent()
{
	return CharacterParametersComponent;
}

bool ADeath_MAZECharacter::SetWallOnTheFloor_Validate()
{
	return true;
}

void ADeath_MAZECharacter::SetWallOnTheFloor_Implementation()
{
	if (GetInventoryComponent()->SpawnOrMakeVisible)
	{

		GetInventoryComponent()->SpawnWallsEvent.Broadcast();
	}
	
	
	
}

bool ADeath_MAZECharacter::GetAccelerationAllow()
{
	return AccelerationAllow;
}

void ADeath_MAZECharacter::SetAccelerationAllow(bool NewState)
{
	this->AccelerationAllow = NewState;

	
}



// call on client side when player pick up item
void ADeath_MAZECharacter::PickUp_Item()
{
	
	ServerPickUp_Item();

}


void ADeath_MAZECharacter::Client_Jump_Implementation()
{
	
	Jump();
}




bool ADeath_MAZECharacter::ServerPickUp_Item_Validate()
{
	return true;
}
// call on server side when player pick up item 
void ADeath_MAZECharacter::ServerPickUp_Item_Implementation()
{

	

	if (HasAuthority())
	{
		
	  TArray<AActor*> CollectedItems;
	// get all overlapping actors
	  CollectionSphere->GetOverlappingActors(CollectedItems);
	  UE_LOG(LogTemp, Warning, TEXT("Take it : %i"), CollectedItems.Num());
	  InventoryComponent->AddItemToInventory(CollectedItems);

	}




}






void ADeath_MAZECharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADeath_MAZECharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
		
}

void ADeath_MAZECharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();

}

void ADeath_MAZECharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADeath_MAZECharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADeath_MAZECharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADeath_MAZECharacter::MoveRight(float Value)
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


void ADeath_MAZECharacter::Acceleration()
{

	Server_Acceleration();
	
}

bool ADeath_MAZECharacter::Server_Acceleration_Validate()
{
	return true;
}

void ADeath_MAZECharacter::Server_Acceleration_Implementation()
{
	if (!isAccelerating)
	{
		isAccelerating = true;
		if (!(GetCharacterParametersComponent()->GetStamina() <= 0))
		{
			isAccelerating = true;
			OffAim();
			SetAccelerationAllow(true);
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterParametersComponent()->MaxWalkSpeed_Acceleration;
			Client_SetMaxWalkSpeed(GetCharacterParametersComponent()->MaxWalkSpeed_Acceleration);
			GetCharacterParametersComponent()->ChangeStaminaAmount = -10.f;
			GetCharacterParametersComponent()->DecreaseStamina();
		}

	}
}

void ADeath_MAZECharacter::AccelerationButton_Released()
{
	Server_AccelerationButton_Released();
}



bool ADeath_MAZECharacter::Server_AccelerationButton_Released_Validate()
{
	return true;
}

void ADeath_MAZECharacter::Server_AccelerationButton_Released_Implementation()
{
	if (isAccelerating)
	{
		isAccelerating = false;
		SetAccelerationAllow(false);
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterParametersComponent()->MaxWalkSpeed_Normal;
		Client_SetMaxWalkSpeed(GetCharacterParametersComponent()->MaxWalkSpeed_Normal);
		GetCharacterParametersComponent()->ChangeStaminaAmount = 10.f;
		GetCharacterParametersComponent()->IncreaseStamina();
	}
}


void ADeath_MAZECharacter::Client_SetMaxWalkSpeed_Implementation(float MaxWalkSpeedParam)
{

	ADeath_MAZECharacter * Character = Cast<ADeath_MAZECharacter>(GetOwner());
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedParam;

}


