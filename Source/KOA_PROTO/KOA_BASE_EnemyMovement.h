// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "KOA_BASE_EnemyMovement.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API UKOA_BASE_EnemyMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:

	//Init
	virtual void InitializeComponent() override;

	//Tick
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
