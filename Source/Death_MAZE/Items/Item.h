// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStruct.h"
#include "Item.generated.h"


UCLASS()
class DEATH_MAZE_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Structure of Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
	FItemStruct ItemStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMesh")
	class UStaticMeshComponent* ItemMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	class UBoxComponent* Box_Trigger;
};
