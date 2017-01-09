// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ATMA_StatusEffects.generated.h"

UENUM(BlueprintType)
enum class EStatusEffectTypes : uint8 {
	SE_BLEED = 0,
	SE_BURN,
	SE_POISON,
	NUM,
	NONE = 99
};

USTRUCT()
struct KOA_PROTO_API FStatusEffectStats {
	GENERATED_USTRUCT_BODY()
public:
	bool  HasStatusEffect;
	float TickAmount;
	float TickFrequency;
public:
	FStatusEffectStats() {
		HasStatusEffect = false;
		TickAmount = 0.0f;
		TickFrequency = 0.0f;
	}
};

USTRUCT()
struct KOA_PROTO_API FStatusEffects {
	GENERATED_USTRUCT_BODY()
public:
	// Bools indicating whether the status effect exists
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	bool HasBleed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	bool HasBurn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	bool HasPoison;

	// Status effect amount per tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick|Amount", meta = (EditCondition = "HasBleed"))
	float BleedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick|Amount", meta = (EditCondition = "HasBurn"))
	float BurnAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick|Amount", meta = (EditCondition = "HasPoison"))
	float PoisonAmount;

	// Tick frequency in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick|Rate", meta = (EditCondition = "HasBleed"))
	float BleedFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick|Rate", meta = (EditCondition = "HasBurn"))
	float BurnFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick|Rate", meta = (EditCondition = "HasPoison"))
	float PoisonFrequency;
	
public:
	FStatusEffects() {
	// init bools
	HasBleed = false;
	HasBurn = false;
	HasPoison = false;
	// init amounts
	BleedAmount = 0.0f;
	BurnAmount = 0.0f;
	PoisonAmount = 0.0f;
	// init frequency
	BleedFrequency = 0.0f;
	BurnFrequency = 0.0f;
	PoisonFrequency = 0.0f;
	}
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};


UCLASS(Blueprintable)
class KOA_PROTO_API UATMA_StatusEffects : public UObject {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture* UIBleedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture* UIBurnIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture* UIPoisonIcon;
public:
	UATMA_StatusEffects();
	virtual ~UATMA_StatusEffects();
	
};
