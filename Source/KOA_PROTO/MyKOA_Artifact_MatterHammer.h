// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KOA_BASE_Artifact.h"
#include "MyKOA_Artifact_MatterHammer.generated.h"

/**
 * 
 */

class AKOA_PROTO_Character;

UCLASS()
class KOA_PROTO_API UKOA_Artifact_MatterHammer : public UKOA_BASE_Artifact
{
	GENERATED_BODY()

public: // Methods
	UKOA_Artifact_MatterHammer(const FObjectInitializer& ObjectInitializer);
	void Tick(float DeltaTime);

	TSubclassOf<AActor> MH_Plat;
	TSubclassOf<AActor> MH_Pill;
	TSubclassOf<AActor> MH_Ball;

	// Basic Attacks
	void UseLightAttack();

	// Press Ability
	void PressAbilityQ();
	void PressAbilityW();
	void PressAbilityE();
	void PressAbilityR();

	// Release Ability
	void ReleaseAbilityQ();
	void ReleaseAbilityW();
	void ReleaseAbilityE();
	void ReleaseAbilityR();

private:
	FVector PlatPos;
	FVector PillPos;
	FVector BallPos;

};
