// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Item_Bounce.generated.h"

/**
 * 
 */
UCLASS()
class DEATH_MAZE_API AItem_Bounce : public AItem
{
	GENERATED_BODY()

		
		
public:
	
	AItem_Bounce();


	UFUNCTION()
		void OnBounceOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ADeath_MAZECharacter * Character;



	int what = 0;
	


};
