// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Wall.h"
#include "Components/BoxComponent.h"


AItem_Wall::AItem_Wall()
{
	bReplicates = true;
	SetReplicateMovement(true);

	
	Box_Trigger->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);




	
	
}
