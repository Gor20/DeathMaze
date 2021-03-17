// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/CharacterParametrs/CharacterParametrs.h"
#include "Death_MAZECharacter.generated.h"


UCLASS(config=Game)
class ADeath_MAZECharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ADeath_MAZECharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Returns Inventory component
	UFUNCTION(BlueprintPure, Category = "Inventory")
	class UInventory_Component* GetInventoryComponent();

	// Returns Inventory component
	UFUNCTION(BlueprintPure, Category = "CharacterParametrs")
	class UCharacterParametrs* GetCharacterParametersComponent();

	// Inventory Component
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	class UInventory_Component* InventoryComponent;

	UFUNCTION(Client, Reliable)
	void Client_Jump();

	UFUNCTION(Reliable, Server, WithValidation)
	void SetWallOnTheFloor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCharacterParametrs* CharacterParametersComponent;

	UFUNCTION()
	bool GetAccelerationAllow();

	UFUNCTION()
	void SetAccelerationAllow(bool NewState);

	UFUNCTION(Reliable, Client)
	void Client_SetMaxWalkSpeed(float MaxWalkSpeedParam);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMeshForItem")
	int MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acceleration")
	bool AccelerationAllow;

	UFUNCTION(BlueprintImplementableEvent)
	void OffAim();

	UPROPERTY(BlueprintReadWrite)
	bool isAccelerating = false;

private:

	// collection sphere for collecting items
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	class USphereComponent* CollectionSphere;


	// call on server side when player pick up item
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerPickUp_Item();

	// call on client side when player pick up item
	UFUNCTION()
	void PickUp_Item();


	UFUNCTION()
	void Acceleration();

	UFUNCTION(Reliable, Server, WithValidation)
	void Server_Acceleration();


	UFUNCTION()
	void AccelerationButton_Released();

	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
	void Server_AccelerationButton_Released();
};
