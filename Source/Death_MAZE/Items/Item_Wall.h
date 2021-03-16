// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Item_Wall.generated.h"



/**
 * 
 */
UCLASS()
class DEATH_MAZE_API AItem_Wall : public AItem
{
	GENERATED_BODY()

		AItem_Wall();


public:
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	class UMaterial * MaterialForWall;

	
};
