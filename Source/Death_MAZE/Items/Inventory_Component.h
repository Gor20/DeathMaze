// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemStruct.h"
#include "Inventory_Component.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVisibleWallsEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnWallsEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEATH_MAZE_API UInventory_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProperty) const override;

	
	//// get item by SlotID
	UFUNCTION()
	FItemStruct GetItem(int SlotID);

	// drop item from inventory
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "InventoryParam")
		void Server_DropItem(const TArray<FItemStruct> & InventoryParam, int SlotIDParam);

	// update SlotsID of inventory 
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "InventoryParam")
		void Server_UpdateInventory(const TArray<FItemStruct> & InventoryParam);
	

	// apply items 
		UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "InventoryParam")
			void Server_ApllyItem(const TArray<FItemStruct>  &InventoryParam, int SlotIDParam, int CountToApplyParam);

	// call on client side when item was pickup
	UFUNCTION(NetMulticast, Unreliable)
		void Client_ItemWasPickUp(APawn * Pawn);

	// call in BP when item was pickup, because function can't be call in BP
	UFUNCTION(BlueprintImplementableEvent)
		void BP_ItemWasPickUp();
	
	// Add Item in inventory when player pick up it
	UFUNCTION()
		void AddItemToInventory(TArray<AActor*> CollectedItems);
	// SubtractionOtItems when player apply item
	UFUNCTION()
		void SubtractionOfItems(int CountToApply);

	// array of ItemStructs that contain items
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
		TArray<FItemStruct> Inventory;

	// elem of array which has to be apply
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	int elemOfArrayToApply;

	UFUNCTION()
		void MakeVisibleWall();

	UPROPERTY()
		class AItem_Wall * ItemWall;


	UPROPERTY(BlueprintReadWrite, Category = "Walls")
		bool SpawnOrMakeVisible = false;

	UPROPERTY()
	class ADeath_MAZECharacter * Character;

	UPROPERTY(BlueprintAssignable, Category = "Walls")
	FOnVisibleWallsEvent VisibleWallsEvent;

	UPROPERTY(BlueprintAssignable, Category = "Walls")
	FOnSpawnWallsEvent SpawnWallsEvent;


};
