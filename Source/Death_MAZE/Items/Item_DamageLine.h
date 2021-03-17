// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Item_DamageLine.generated.h"

UCLASS()
class DEATH_MAZE_API AItem_DamageLine : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem_DamageLine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetDamageAmount(int DamageAmountParam);

	UFUNCTION()
	int getDamageAmount();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnDamageLineOverlap(UPrimitiveComponent* OverlappedComponent,
	                         AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex,
	                         bool bFromSweep,
	                         const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	int DamageAmount;

private:

	UPROPERTY(EditAnywhere)
	class ADeath_MAZECharacter* Character;
};
