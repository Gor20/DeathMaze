// Fill out your copyright notice in the Description page of Project Settings.

#include "Item_DamageLine.h"
#include "Death_MAZE/Death_MAZECharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
AItem_DamageLine::AItem_DamageLine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItem_DamageLine::OnDamageLineOverlap);
}

// Called when the game starts or when spawned
void AItem_DamageLine::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AItem_DamageLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AItem_DamageLine::SetDamageAmount(int DamageAmountParam)
{
	this->DamageAmount = DamageAmountParam;
}

int AItem_DamageLine::getDamageAmount()
{
	return DamageAmount;
}

void AItem_DamageLine::OnDamageLineOverlap(UPrimitiveComponent* OverlappedComponent,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex,
                                           bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		Character = Cast<ADeath_MAZECharacter>(OtherActor);

		int Health = Character->GetCharacterParametersComponent()->GetHealth();
		if (Health <= 0) return;
		Character->GetCharacterParametersComponent()->ChangeHealth(20);
		Character->GetCharacterParametersComponent()->Client_DecreaseHealthNotify();
	}
}
