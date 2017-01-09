// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KOA_BASE_EnemyCharacter.h"
#include "AMTA_BASE_BossCharacter.generated.h"

/**
 * 
 */
UCLASS()
class KOA_PROTO_API AAMTA_BASE_BossCharacter : public AKOA_BASE_EnemyCharacter
{
	GENERATED_BODY()
	
public:
	UFUNCTION(exec, BlueprintCallable, BlueprintNativeEvent, meta=(DisplayName="Amentia Console cmd: CE_TEST"), Category = "Events")
	void CE_TEST();
	
};
