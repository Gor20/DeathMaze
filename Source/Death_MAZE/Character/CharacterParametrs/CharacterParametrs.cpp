// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterParametrs.h"
#include "Net/UnrealNetwork.h"
#include "Death_MAZE/Death_MAZECharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCharacterParametrs::UCharacterParametrs()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100;
	Stamina = 100;

	MaxWalkSpeed_Acceleration = 700.f;
	MaxWalkSpeed_Normal = 400.f;

	DelayForDecreaseStamina_Timer = 1.f;
	DelayForIncreaseStamina_Timer = 1.f;

}


void UCharacterParametrs::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterParametrs, Health);
	DOREPLIFETIME(UCharacterParametrs, Stamina);

}

// Called when the game starts
void UCharacterParametrs::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	// ...

}


// Called every frame
void UCharacterParametrs::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UCharacterParametrs::GetHealth()
{
	return Health;
}

int UCharacterParametrs::GetStamina()
{
	return Stamina;
}



void UCharacterParametrs::SetHealth(int HealthParam)
{
	this->Health = HealthParam;
}

void UCharacterParametrs::SetStamina(int StaminaParam)
{
	this->Stamina = StaminaParam;
}

void UCharacterParametrs::ChangeHealth(int AmountParam)
{

	this->Health -= AmountParam;
}

void UCharacterParametrs::DecreaseStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(IncreaseStamina_Timer);
	GetWorld()->GetTimerManager().SetTimer(DecreaseStamina_Timer, this, &UCharacterParametrs::DecreaseStamina_TimerFunc, DelayForDecreaseStamina_Timer, true);
}

void UCharacterParametrs::DecreaseStamina_TimerFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("acceleration :%i"), GetStamina());
	if (GetStamina() <= 0)
	{
		
		ADeath_MAZECharacter * Character = Cast<ADeath_MAZECharacter>(GetOwner());

		GetWorld()->GetTimerManager().ClearTimer(DecreaseStamina_Timer);
		Character->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Acceleration;
		Character->Client_SetMaxWalkSpeed(MaxWalkSpeed_Acceleration);
		Character->SetAccelerationAllow(false);
		
	}
	else
	{
		
		SetStamina(GetStamina() + ChangeStaminaAmount);
	}
	

}




void UCharacterParametrs::IncreaseStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(DecreaseStamina_Timer);
	GetWorld()->GetTimerManager().SetTimer(IncreaseStamina_Timer, this, &UCharacterParametrs::IncreaseStamina_TimerFunc, DelayForIncreaseStamina_Timer, true);

	ADeath_MAZECharacter * Character = Cast<ADeath_MAZECharacter>(GetOwner());


}

void UCharacterParametrs::IncreaseStamina_TimerFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("acceleration :%i"), GetStamina());
	if (GetStamina() >= 100)
	{
		GetWorld()->GetTimerManager().ClearTimer(IncreaseStamina_Timer);
	}
	else
	{
		SetStamina(GetStamina() + ChangeStaminaAmount);
	}
	
}





void UCharacterParametrs::Client_DecreaseHealthNotify_Implementation()
{
	ADeath_MAZECharacter * character = Cast<ADeath_MAZECharacter>(GetOwner());
	
		DecreaseHealthNotify.Broadcast();
		
		UE_LOG(LogTemp, Warning, TEXT("electric :%i"), character->GetCharacterParametersComponent()->GetHealth());
		
	
}
		



