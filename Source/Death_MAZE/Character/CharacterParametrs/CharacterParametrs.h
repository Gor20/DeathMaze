// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterParametrs.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInterfaceDecreaseHealthEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEATH_MAZE_API UCharacterParametrs : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterParametrs();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProperty) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	int GetHealth();

	UFUNCTION(BlueprintCallable)
	void SetHealth(int HealthParam);

	UFUNCTION(BlueprintPure)
	int GetStamina();

	UFUNCTION()
	void SetStamina(int StaminaParam);

	UFUNCTION()
	void ChangeHealth(int AmountParam);

	UFUNCTION()
	void DecreaseStamina();

	UFUNCTION()
	void IncreaseStamina();

	UFUNCTION()
	void DecreaseStamina_TimerFunc();

	UFUNCTION()
	void IncreaseStamina_TimerFunc();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_DecreaseHealthNotify();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInterfaceDecreaseHealthEvent DecreaseHealthNotify;

	FTimerHandle DecreaseStamina_Timer;

	FTimerHandle IncreaseStamina_Timer;

	int ChangeStaminaAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParameters")
	float MaxWalkSpeed_Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParameters")
	float MaxWalkSpeed_Normal;

private:

	UPROPERTY(Replicated)
	int Health;

	UPROPERTY(Replicated)
	int Stamina;

	int DelayForDecreaseStamina_Timer;
	int DelayForIncreaseStamina_Timer;
};
