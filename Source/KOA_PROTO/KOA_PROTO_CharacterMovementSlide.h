// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "KOA_PROTO_CharacterMovementSlide.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API UKOA_PROTO_CharacterMovementSlide : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:

	//Init
	virtual void InitializeComponent() override;

	//Tick
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

};
