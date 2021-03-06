// Fill out your copyright notice in the Description page of Project Settings.

#include "Item_Bounce.h"
#include "Item.h"
#include "Components/BoxComponent.h"
#include "Death_MAZE/Death_MAZECharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AItem_Bounce::AItem_Bounce()
{
}

void AItem_Bounce::OnBounceOverlap(UPrimitiveComponent* OverlappedComponent,
                                   AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex,
                                   bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		Character = Cast<ADeath_MAZECharacter>(OtherActor);

		Character->GetCharacterMovement()->JumpZVelocity = 1000.f;
		Character->GetCharacterMovement()->BrakingDecelerationFalling = 6;
		Character->Client_Jump();
	}
}
