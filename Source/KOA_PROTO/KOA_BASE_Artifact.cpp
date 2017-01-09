// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_BASE_Artifact.h"
#include "KOA_PROTO_Character.h"

UKOA_BASE_Artifact::UKOA_BASE_Artifact() {
	// STATS //
	ArtifactID = EArtifactID::ID_NULL;
	ArtifactName = "INVALID";
	ArtifactIconTexture = nullptr;
	ArtifactMesh = nullptr;
	LightBasicAttackLockDuration = 5.0f;
	// ABILITY //
	IsArtifactStormUnlocked = false;
	
	// Initialize private member variables //
	PlayerReference = nullptr;
	CurrentHeldAbilityButton = EAbilityID::NONE;
	IsBasicAttackOnCooldown = false;
	BasicAttackInUse = EBasicAttack::NONE;
	
	FAbilityTimerHandles(AbilityQTimer, AbilityWTimer, AbilityETimer, AbilityRTimer, BasicAttackTimer);
}


	
UKOA_BASE_Artifact::~UKOA_BASE_Artifact() {}

void UKOA_BASE_Artifact::UseLightAttack() {
//	GetPlayerReference()->SetCanDodge(false);
	
	IsBasicAttackOnCooldown = true;
	BasicAttackInUse = EBasicAttack::LIGHT;
	StartBasicAttackCooldownTimer(EBasicAttack::LIGHT);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "BASE_Artifact::BasicAttack Cooldown started.");
}

void UKOA_BASE_Artifact::PressAbilityQ() {}
void UKOA_BASE_Artifact::PressAbilityW() {}
void UKOA_BASE_Artifact::PressAbilityE() {}
void UKOA_BASE_Artifact::PressAbilityR() {}

void UKOA_BASE_Artifact::ReleaseAbilityQ() {}
void UKOA_BASE_Artifact::ReleaseAbilityW() {}
void UKOA_BASE_Artifact::ReleaseAbilityE() {}
void UKOA_BASE_Artifact::ReleaseAbilityR() {}

// RESET BASIC ATTACK //
void UKOA_BASE_Artifact::ResetBasicAttackCooldown() {
	BasicAttackInUse = EBasicAttack::NONE;
	IsBasicAttackOnCooldown = false;
	GetPlayerReference()->SetIsMovementInputDisabled(false);
	//GetPlayerReference()->SetCanDodge(true);
}
void UKOA_BASE_Artifact::UninitializeBasicAttacks() {
	BasicAttackInUse = EBasicAttack::NONE;
	IsBasicAttackOnCooldown = false;
}
// RESET ABILITY //
void UKOA_BASE_Artifact::ResetAbilityQCooldown() {
	AbilityQ.ResetAbilityCooldown();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Reset Current Q timer");
}
void UKOA_BASE_Artifact::ResetAbilityWCooldown() {
	AbilityW.ResetAbilityCooldown();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Reset Current W timer");
}
void UKOA_BASE_Artifact::ResetAbilityECooldown() {
	AbilityE.ResetAbilityCooldown();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Reset Current E timer");
}
void UKOA_BASE_Artifact::ResetAbilityRCooldown() {
	AbilityR.ResetAbilityCooldown();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Reset Current R timer");
}

void UKOA_BASE_Artifact::StartAbilityCooldownTimer(EAbilityID AbilityID) {
	// When the timer ends, unlock ability use
	if (GetPlayerReference()->GetWorldPtr()) {
		GetPlayerReference()->StartAbilityLockTimer();

		switch (AbilityID) {
		case EAbilityID::ABID_Q:
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "StartAbilityCooldownTimer: ABID_Q");
			GetPlayerReference()->GetWorldPtr()->GetTimerManager().SetTimer(this->AbilityQTimer, this, &UKOA_BASE_Artifact::ResetAbilityQCooldown, this->AbilityQ.AbilityCooldownDuration, false);
			break;
		case EAbilityID::ABID_W:
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "StartAbilityCooldownTimer: ABID_W");
			GetPlayerReference()->GetWorldPtr()->GetTimerManager().SetTimer(this->AbilityWTimer, this, &UKOA_BASE_Artifact::ResetAbilityWCooldown, this->AbilityW.AbilityCooldownDuration, false);
			break;
		case EAbilityID::ABID_E:
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "StartAbilityCooldownTimer: ABID_E");
			GetPlayerReference()->GetWorldPtr()->GetTimerManager().SetTimer(this->AbilityETimer, this, &UKOA_BASE_Artifact::ResetAbilityECooldown, this->AbilityE.AbilityCooldownDuration, false);
			break;		
		case EAbilityID::ABID_R:
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "StartAbilityCooldownTimer: ABID_R");
			GetPlayerReference()->GetWorldPtr()->GetTimerManager().SetTimer(this->AbilityRTimer, this, &UKOA_BASE_Artifact::ResetAbilityRCooldown, this->AbilityR.AbilityCooldownDuration, false);
			break;
		default:
			break;
		}
	}
}

void UKOA_BASE_Artifact::StartBasicAttackCooldownTimer(EBasicAttack TypeOfBA) {
	// When the timer ends, you can basic attack again.
	if (GetPlayerReference()->GetWorldPtr()) {
		switch (TypeOfBA) {
		case EBasicAttack::LIGHT:
			GetPlayerReference()->GetWorldPtr()->GetTimerManager().SetTimer(BasicAttackTimer,this, &UKOA_BASE_Artifact::ResetBasicAttackCooldown, this->LightBasicAttackLockDuration, false);
			break;
		case EBasicAttack::HEAVY:
			break;
		default:
			break;
		}
	}
}

void UKOA_BASE_Artifact::ClearAllTimers() {
	//TODO: All the  timers. Get the q working first damnit.
	
}
/*-- GETTERS --*/
AKOA_PROTO_Character* UKOA_BASE_Artifact::GetPlayerReference() {
	if (PlayerReference == nullptr) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "FATAL ERROR: VampDaggers PlayerRef* == nullptr");
		return nullptr;
	} else {
		return PlayerReference;
	}
}
/*-- SETTERS --*/
void UKOA_BASE_Artifact::SetBasicAttackInUse(EBasicAttack TypeOfBA) {
	BasicAttackInUse = TypeOfBA;
}
void UKOA_BASE_Artifact::SetCurrentHeldAbilityButton(EAbilityID ability) {
	CurrentHeldAbilityButton = ability;
}

void UKOA_BASE_Artifact::SetPlayerReference(AKOA_PROTO_Character* player) {
	PlayerReference = player;
}