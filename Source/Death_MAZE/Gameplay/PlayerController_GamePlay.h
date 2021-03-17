// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_GamePlay.generated.h"

/**
 * 
 */
UCLASS()
class DEATH_MAZE_API APlayerController_GamePlay : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	APlayerController_GamePlay();
};
