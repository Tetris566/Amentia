// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KOA_BASE_Artifact.h"
#include "KOA_Artifact_FireGlove.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API UKOA_Artifact_FireGlove : public UKOA_BASE_Artifact
{
	GENERATED_BODY()

	UKOA_Artifact_FireGlove();

	void UseLightAttack();
	void PressAbilityQ();
	void PressAbilityW();
	void PressAbilityE();
};
