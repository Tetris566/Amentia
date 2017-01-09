// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ATMA_StatusEffects.h"
#include "KOA_BASE_Artifact.generated.h"

// Forward Declaration of classes
class AKOA_PROTO_Character;

UENUM(BlueprintType)
enum class EArtifactID : uint8 {
	ID_DualDaggers     = 0,
	ID_FireGlove       ,
	ID_MatterHammer    ,
	ID_WindSpear       ,
	NUM,
	ID_NULL = 99,
};

UENUM(BlueprintType)
enum class EAbilityID : uint8 {
	ABID_Q = 0,
	ABID_W,
	ABID_E,
	ABID_R,
	NONE = 99
};

UENUM(BlueprintType)
enum class EBasicAttack : uint8 {
	LIGHT = 0,
	HEAVY,
	NUM,
	NONE = 99
};

USTRUCT()
struct KOA_PROTO_API FAbilityStats {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString AbilityName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture* AbilityIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HitDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AbilityCooldownDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxCastRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool AbilityOnCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffects")
	FStatusEffects StatusEffects;
public:
	FAbilityStats() {
		AbilityName = "INVALID NAME";
		AbilityIconTexture = nullptr;
 		HitDamage = 0.0f;
 	    MaxCastRange = 0.0f;
 		AbilityCooldownDuration = 0.0f;
        AbilityOnCooldown = false;
	}
	
	void operator=(const FAbilityStats& Stats) {
		this->AbilityName = Stats.AbilityName;
		this->AbilityIconTexture = Stats.AbilityIconTexture;
 		this->HitDamage = Stats.HitDamage;
 		this->MaxCastRange = Stats.MaxCastRange;
 		this->AbilityCooldownDuration = Stats.AbilityCooldownDuration;
 		this->AbilityOnCooldown = Stats.AbilityOnCooldown;
		this->StatusEffects = Stats.StatusEffects;
	}
};

USTRUCT()
struct KOA_PROTO_API FAbility {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString AbilityName;

	UPROPERTY(EditAnywhere, Category = "Texture")
	UTexture* AbilityIconTexture;
	
	UPROPERTY(EditAnywhere, Category = "Damage")
 	float HitDamage;
	
	UPROPERTY(EditAnywhere, Category = "Cast Range")
	float MaxCastRange;

	UPROPERTY(EditAnywhere, Category = "Timer", DisplayName = "Ability Cooldown")
	float AbilityCooldownDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Cooldown", DisplayName = "Ability Cooldown")
	bool AbilityOnCooldown;
	FTimerHandle AbilityCooldownTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffects")
	FStatusEffects StatusEffects;
public:
	FAbility() {
		AbilityName = "INVALID";
		HitDamage = 0.0f;
		MaxCastRange = 0.0f;
		AbilityCooldownDuration = 0.0f;
		AbilityOnCooldown = false;
	}

	FORCEINLINE bool IsAbilityOnCooldown() const {
		return AbilityOnCooldown;
	}
	
	void SetAbilityOnCooldown() {
		AbilityOnCooldown = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, AbilityName + TEXT(" cooldown set!"));
	}
	void ResetAbilityCooldown() {
		AbilityOnCooldown = false;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, AbilityName + TEXT(" cooldown reset!"));
	}
	
	FAbilityStats GetAbilityStats() const {
		FAbilityStats stats;
		stats.AbilityName = this->AbilityName;
		stats.AbilityIconTexture = this->AbilityIconTexture;
		stats.HitDamage = this->HitDamage;
		stats.MaxCastRange = this->MaxCastRange;
		stats.AbilityCooldownDuration = this->AbilityCooldownDuration;
		stats.AbilityOnCooldown = this->AbilityOnCooldown;
		stats.StatusEffects = this->StatusEffects;
		return stats;
	}
};

USTRUCT()
struct KOA_PROTO_API FAbilityTimerHandles {
	GENERATED_USTRUCT_BODY()
public:
	FTimerHandle LightAttackTimer;
	FTimerHandle AbilityQTimer;
	FTimerHandle AbilityWTimer;
	FTimerHandle AbilityETimer;
	FTimerHandle AbilityRTimer;
	
public:
	FAbilityTimerHandles() {}
	FAbilityTimerHandles(FTimerHandle Q, FTimerHandle W, FTimerHandle E, FTimerHandle R, FTimerHandle LightAttack)
		: AbilityQTimer(Q), AbilityWTimer(W), AbilityETimer(E), AbilityRTimer(R), LightAttackTimer(LightAttack) {}
		
	void operator=(const FAbilityTimerHandles& Handles) {
		this->LightAttackTimer = Handles.LightAttackTimer;
		this->AbilityQTimer = Handles.AbilityQTimer;
		this->AbilityWTimer = Handles.AbilityWTimer;
 		this->AbilityETimer = Handles.AbilityETimer;
 		this->AbilityRTimer = Handles.AbilityRTimer;
	}
	
};

UCLASS(Blueprintable)
class KOA_PROTO_API UKOA_BASE_Artifact : public UObject
{
	GENERATED_BODY()
public:
	//** STATS **//
	UPROPERTY(BlueprintReadWrite, Category = "Artifact")
	EArtifactID ArtifactID;
	UPROPERTY(EditAnywhere, Category = "Stats")
	FString ArtifactName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture* ArtifactIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* ArtifactMesh;
	UPROPERTY(EditAnywhere, Category = "Stats|Combat")
	float LightBasicAttackLockDuration;
	//UPROPERTY(EditAnywhere, Category = "Stats|Combat")
	//float HeavyBasicAttackLockDuration;
	UPROPERTY(EditAnywhere, Category = "Stats|Combat|StatusEffects")
	FStatusEffects BasicAttackStatusEffects;
	//** ABILITIES **//
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityQ;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityW;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityE;
	UPROPERTY(EditAnywhere, Category = "Ability")
	FAbility AbilityR;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Storm")
	bool IsArtifactStormUnlocked;
	//** TIMERS **//
	FTimerHandle BasicAttackTimer;
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Timer")
	FTimerHandle AbilityQTimer;
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Timer")
	FTimerHandle AbilityWTimer;
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Timer")
	FTimerHandle AbilityETimer;
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Timer")
	FTimerHandle AbilityRTimer;
	
	FAbilityTimerHandles AbilityTimerHandles;
	
public:
	UKOA_BASE_Artifact();
	virtual ~UKOA_BASE_Artifact();
	// TICK //
	virtual void Tick(float DeltaTime) {}
	// USE BASIC ATTACK //
	virtual void UseLightAttack();
	// PRESS ABILITY //
	virtual void PressAbilityQ();
	virtual void PressAbilityW();
	virtual void PressAbilityE();
	virtual void PressAbilityR();
	// RELEASE ABILITY //
	virtual void ReleaseAbilityQ();
	virtual void ReleaseAbilityW();
	virtual void ReleaseAbilityE();
	virtual void ReleaseAbilityR();
	// RESET BASIC ATTACK //
	void ResetBasicAttackCooldown();
	void UninitializeBasicAttacks();
	// RESET ABILITY //
	void ResetAbilityQCooldown();
	void ResetAbilityWCooldown();
	void ResetAbilityECooldown();
	void ResetAbilityRCooldown();
	// TIMERS //
	void StartAbilityCooldownTimer(EAbilityID AbilityID);
	void StartBasicAttackCooldownTimer(EBasicAttack TypeOfBA);
	void ClearAllTimers();
	
	/*-- GETTERS --*/ 
	FORCEINLINE bool GetIsBasicAttackOnCooldown() const {
		return IsBasicAttackOnCooldown;
	}
	UFUNCTION(BlueprintCallable, Category = "BasicAttack")
	EBasicAttack GetBasicAttackInUse() const {
		return BasicAttackInUse;
	}
	UFUNCTION(BlueprintCallable, Category = "Ability|Stats")
	FAbilityStats GetAbilityStats(EAbilityID Ability) const {
		FAbilityStats stats;
		switch(Ability) {
		case EAbilityID::ABID_Q:
			stats = AbilityQ.GetAbilityStats();
			break;
		case EAbilityID::ABID_W:
			stats = AbilityW.GetAbilityStats();
			break;
		case EAbilityID::ABID_E:
			stats = AbilityE.GetAbilityStats();
			break;
		case EAbilityID::ABID_R:
			stats = AbilityR.GetAbilityStats();
			break;
		default:
			break;
		}
		return stats;
	}
	
	FORCEINLINE FAbilityTimerHandles GetArtifactAbilityTimerHandles() const {
		return AbilityTimerHandles;
	}
	FORCEINLINE EAbilityID GetCurrentHeldAbilityButton() const {
		return CurrentHeldAbilityButton;
	}
	AKOA_PROTO_Character* GetPlayerReference();

	/*-- SETTERS --*/ 
	UFUNCTION(BlueprintCallable, Category = "BasicAttack")
	void SetBasicAttackInUse(EBasicAttack TypeOfBA);
	void SetCurrentHeldAbilityButton(EAbilityID ability);
	void SetPlayerReference(AKOA_PROTO_Character* player);

private: // Variables
	AKOA_PROTO_Character* PlayerReference;
	EAbilityID CurrentHeldAbilityButton;
	bool IsBasicAttackOnCooldown;
	EBasicAttack BasicAttackInUse;
};
