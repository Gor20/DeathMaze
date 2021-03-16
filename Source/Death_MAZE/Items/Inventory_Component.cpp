// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory_Component.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ItemStruct.h"
#include "Death_MAZE/Death_MAZECharacter.h"
#include "Item.h"
#include "Item_Wall.h"
#include "Engine/EngineTypes.h"


// Sets default values for this component's properties
UInventory_Component::UInventory_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInventory_Component::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}


// Called every frame
void UInventory_Component::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Newloc;
	//Newloc.X = Character->GetActorLocation().X + 300.f;
	//Newloc.Y = Character->GetActorLocation().Y;
	//	Newloc.Z = Character->GetActorLocation().Z;

	ItemWall->SetActorLocation(Character->GetActorLocation() + Character->GetActorForwardVector() + 300.f);
	ItemWall->SetActorRotation(Character->GetActorRotation());


	UE_LOG(LogTemp, Warning, TEXT("ActorTick :%f"), ItemWall->GetActorLocation().X);
	UE_LOG(LogTemp, Warning, TEXT("ActorTick :%f"), ItemWall->GetActorLocation().Y);
	UE_LOG(LogTemp, Warning, TEXT("ActorTick :%f"), ItemWall->GetActorLocation().Z);
}


void UInventory_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventory_Component, Inventory);
	DOREPLIFETIME(UInventory_Component, elemOfArrayToApply);
}


FItemStruct UInventory_Component::GetItem(int ByElemOfArray)
{
	
	return Inventory[ByElemOfArray];
}


bool UInventory_Component::Server_UpdateInventory_Validate(const TArray<FItemStruct>& InventoryParam)
{
	return true;
}

void UInventory_Component::Server_UpdateInventory_Implementation(const TArray<FItemStruct>& InventoryParam)
{
	for (int i = 0; i < InventoryParam.Num(); i++)
	{
		this->Inventory[i].SlotID = InventoryParam[i].SlotID;
	}
}


bool UInventory_Component::Server_DropItem_Validate(const TArray<FItemStruct>& InventoryParam, int SlotIDParam)
{
	return true;
}

void UInventory_Component::Server_DropItem_Implementation(const TArray<FItemStruct>& InventoryParam, int SlotIDParam)
{
	//UpdateInventory(InventoryParam);
	Server_UpdateInventory(InventoryParam);

	/*APawn * PawnInstigator = Cast<APawn>(GetOwner());
	if (!PawnInstigator) return;
	UE_LOG(LogTemp, Warning, TEXT("item count ++ : %s"), *PawnInstigator->GetName());*/


	int elemOfArrayToDrop = 0;

	// find elem or array which has to be dropped
	for (int i = 0; i < this->Inventory.Num(); i++)
	{
		if (this->Inventory[i].SlotID == SlotIDParam)
		{
			elemOfArrayToDrop = i;
			UE_LOG(LogTemp, Warning, TEXT(" : %i"), elemOfArrayToDrop);
			break;
		}
	}

	// get params to spawn Item


	Character = Cast<ADeath_MAZECharacter>(GetOwner());
	FVector LocationToSpawn = FVector(Character->GetActorLocation() + Character->GetActorForwardVector() * 200);
	FRotator RotationToSpawn = FRotator(Character->GetActorRotation());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UE_LOG(LogTemp, Warning, TEXT("item count ++ : %s"), *Character->GetName());

	// spawn Item in the world consider count of items
	for (int i = 0; i < InventoryParam[elemOfArrayToDrop].Count; i++)
	{
		GetWorld()->SpawnActor<AActor>(GetItem(elemOfArrayToDrop).Class, LocationToSpawn, RotationToSpawn, SpawnParams);
	}

	// clean elem of array
	this->Inventory[elemOfArrayToDrop].SlotID = -1;

	this->Inventory[elemOfArrayToDrop].Type = ItemType::None;
	this->Inventory[elemOfArrayToDrop].Exists = false;
}

bool UInventory_Component::Server_ApllyItem_Validate(const TArray<FItemStruct>& InventoryParam, int SlotIDParam,
                                                     int CountToApplyParam)
{
	
	return true;
}

void UInventory_Component::Server_ApllyItem_Implementation(const TArray<FItemStruct>& InventoryParam, int SlotIDParam,
                                                           int CountToApplyParam)
{
	//UpdateInventory(InventoryParam);
	Server_UpdateInventory(InventoryParam);


	// find elem or array which has to be dropped
	for (int i = 0; i < this->Inventory.Num(); i++)
	{
		if (this->Inventory[i].SlotID == SlotIDParam)
		{
			elemOfArrayToApply = i;
			UE_LOG(LogTemp, Warning, TEXT("here :%i"), elemOfArrayToApply);
			UE_LOG(LogTemp, Warning, TEXT("CountBefore :%i"), this->Inventory[elemOfArrayToApply].Count);
			break;
		}
	}

	// Subtraction required amount
	SubtractionOfItems(CountToApplyParam);


	if (InventoryParam[elemOfArrayToApply].Type.GetValue() == ItemType::Wall)
	{
		// make visible Wall for player
		SpawnOrMakeVisible = false;
		MakeVisibleWall();
	}


	if (this->Inventory[elemOfArrayToApply].Count == 0)
	{
		// clean elem of array
		this->Inventory[elemOfArrayToApply].SlotID = -1;

		this->Inventory[elemOfArrayToApply].Type = ItemType::None;
		this->Inventory[elemOfArrayToApply].Exists = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("End :%i"), this->Inventory[elemOfArrayToApply].Count);
}

void UInventory_Component::SubtractionOfItems(int CountToApply)
{
	if (this->Inventory[elemOfArrayToApply].Count == CountToApply)

	{
		this->Inventory[elemOfArrayToApply].Count -= CountToApply;
		UE_LOG(LogTemp, Warning, TEXT("CountAfter :%i"), this->Inventory[elemOfArrayToApply].Count);
		return;
	}

	if (this->Inventory[elemOfArrayToApply].Count > CountToApply)
	{
		this->Inventory[elemOfArrayToApply].Count -= CountToApply;
		UE_LOG(LogTemp, Warning, TEXT("CountAfter :%i"), this->Inventory[elemOfArrayToApply].Count);
		return;
	}

	else
	{
		this->Inventory[elemOfArrayToApply].Count = 0;
	}
}

void UInventory_Component::MakeVisibleWall()
{
	VisibleWallsEvent.Broadcast();


	UE_LOG(LogTemp, Warning, TEXT("Good"));
}


void UInventory_Component::AddItemToInventory(TArray<AActor*> CollectedItems)
{
	// Get Pawn which pick up Item
	APawn* PawnInstigator = Cast<APawn>(GetOwner());
	if (!PawnInstigator) return;
	UE_LOG(LogTemp, Warning, TEXT("here"));
	// Check all actors and cast to type item
	for (int i = 0; CollectedItems.Num() > i; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewItem"));
		//  UE_LOG(LogTemp, Warning, TEXT("it"));
		AItem* Item = Cast<AItem>(CollectedItems[i]);
		if (!Item) break;

		// if in inventory doesn't exist  Type of new item so we will add it;
		bool AddNewItemTypeOrNot = true;

		for (int j = 0; Inventory.Num() > j; j++)
		{
			// Find type of Item
			if (Item->ItemStruct.Type.GetValue() == Inventory[j].Type.GetValue())
			{
				// increment count of item if item already exists
				UE_LOG(LogTemp, Warning, TEXT("item count ++ : %s"), *PawnInstigator->GetName());
				Inventory[j].Count++;

				// call function on client side 
				Client_ItemWasPickUp(PawnInstigator);
				AddNewItemTypeOrNot = false;
				break;
			}
		}


		// check Inventory elem to not try to get nullpnt index

		// if inventory was checked and new item type wasnt found we add it as new type item
		if (AddNewItemTypeOrNot)
		{
			UE_LOG(LogTemp, Warning, TEXT("Here 2"));

			// show add or not
			bool AddNewElemOrNot = true;
			int k = 0;

			// if elem(item) of array doesn't exist we assian that elem(Item) the new elem(Item)
			for (k; k < Inventory.Num(); k++)
			{
				if (!Inventory[k].Exists == true)
				{
					Inventory[k] = Item->ItemStruct;

					AddNewElemOrNot = false;
					break;
				}
			}
			// if all elems in array exist we add new elem in array
			if (AddNewElemOrNot)
			{
				Inventory.Add(Item->ItemStruct);
			}


			// define SlotID for picked up item
			for (int SlotID = 0; SlotID < 32; SlotID++)
			{
				bool SlotBusy = false;
				for (int j = 0; j < Inventory.Num(); j++)
				{
					// if in Inventory wasn't found Slot which is for loop iterator, then we can assian that slot
					// becase slot isn't busy
					if (Inventory[j].SlotID == SlotID)
					{
						SlotBusy = true;
						break;
					}
					// check for ending array and if array is end and slot is free we can assign him
					if (Inventory.Num() - 1 == j)
					{
						// if we added new elem in array so we have set SlotID to last elem of array
						if (AddNewElemOrNot)
						{
							int LastIndex = Inventory.Num() - 1;
							Inventory[LastIndex].SlotID = SlotID;
						}
						else
						{
							// if we doesn't create new elem, it means we used elem with bool Exist = false, and we
							// assign that elem our picked up item
							Inventory[k].SlotID = SlotID;
						}

						break;
					}
				}

				if (!SlotBusy) break;
			}
		}

		Item->Destroy();

		// and call call function on client side 
		Client_ItemWasPickUp(PawnInstigator);
	}
}


void UInventory_Component::Client_ItemWasPickUp_Implementation(APawn* Pawn)
{
	BP_ItemWasPickUp();
}
