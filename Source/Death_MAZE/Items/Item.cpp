// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	Box_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//	Box_Trigger->bGenerateOverlapEvents = true;

	Box_Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box_Trigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box_Trigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Box_Trigger->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Box_Trigger->SetWorldScale3D(FVector(1.0f));
	Box_Trigger->SetRelativeScale3D(FVector(1.0f));

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMeshComponent;
	Box_Trigger->SetupAttachment(ItemMeshComponent);

	ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ItemMeshComponent->SetMobility(EComponentMobility::Movable);
	ItemMeshComponent->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
