// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TextureResource.h"
#include "Components/ActorComponent.h"
#include "ItemStruct.generated.h"



UENUM()
enum ItemType
{
	None,
	Weapon,
	Money,
	BatteryBIG,
	BatterySmall,
	Wall,

};


USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		bool Exists = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		UTexture2D * Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		TEnumAsByte<ItemType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		int SlotID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		TSubclassOf<AActor> Class;




};



