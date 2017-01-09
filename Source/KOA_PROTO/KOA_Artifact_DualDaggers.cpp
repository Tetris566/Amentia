// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_Artifact_DualDaggers.h"
#include "KOA_PROTO_Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UTIL_MouseFunctionality.h"

UKOA_Artifact_DualDaggers::UKOA_Artifact_DualDaggers(const FObjectInitializer& ObjectInitializer) 
	: Super() {
	ArtifactID = EArtifactID::ID_DualDaggers;
	ArtifactName = "Dual Daggers of Something, Probably";
	LightBasicAttackLockDuration = 1.0f;

	// ABILITY Q //
	AbilityQ.AbilityName = "Stabby Heal-y";
	AbilityQ.AbilityCooldownDuration = 5.0f;
	AbilityQ.MaxCastRange = 100.0f;
	ABILQ_HealAmount = 42.0f;

	// ABILITY W //
	AbilityW.AbilityCooldownDuration = 10.0f;
	AbilityW.MaxCastRange = 150.0f;

	// ABILITY E //
	AbilityE.MaxCastRange = 500.0f; 
	
	
	// ABILITY R //
}
//********** USE BASIC ATTACKS **********//
void UKOA_Artifact_DualDaggers::UseLightAttack() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::LightAttack");
	Super::UseLightAttack();
}

//********** PRESS ABILITY **********//
void UKOA_Artifact_DualDaggers::PressAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::Q");
	AbilityQ.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_Q);
}
void UKOA_Artifact_DualDaggers::PressAbilityW() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::W");
	AbilityW.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_W);
}
void UKOA_Artifact_DualDaggers::PressAbilityE() {
	AKOA_PROTO_Character* player = GetPlayerReference();
	player->VD_E_AimingMeshComponent->SetVisibility(true);
	player->SetIsMovementInputDisabled(true);
}
void UKOA_Artifact_DualDaggers::PressAbilityR() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed DualDagger::R");
}

//********** RELEASE ABILITY **********//
void UKOA_Artifact_DualDaggers::ReleaseAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released DualDagger::Q");
}
void UKOA_Artifact_DualDaggers::ReleaseAbilityW() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released DualDagger::W");
	//StartAbilityCooldownTimer(EAbilityID::ABID_W);
}
void UKOA_Artifact_DualDaggers::ReleaseAbilityE() {
	// Set the aiming mesh component visibility to false
	AKOA_PROTO_Character* player = GetPlayerReference();
	E_LocationSwap();
	player->VD_E_AimingMeshComponent->SetVisibility(false);
	player->SetIsMovementInputDisabled(false);
	StartAbilityCooldownTimer(EAbilityID::ABID_E);
}
void UKOA_Artifact_DualDaggers::ReleaseAbilityR() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released DualDagger::R");
	StartAbilityCooldownTimer(EAbilityID::ABID_R);
}

//******************** TICK ********************//
void UKOA_Artifact_DualDaggers::Tick(float DeltaTime) {
	switch (GetCurrentHeldAbilityButton()) {
	case EAbilityID::ABID_Q:  
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using Dagger: Q");
		break;
	case EAbilityID::ABID_W:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using Dagger: W");
		break;
	case EAbilityID::ABID_E:
	{
		// Get a reference to the player
		AKOA_PROTO_Character* player = GetPlayerReference();
		// Get the mousePos using the UTILITY function
		FVector mousePos = UTIL_MouseFunctionality::GetMousePosInPlayerPlane(player->GetWorldPtr());

		// Get the playerPos
		FVector playerPos = player->GetActorLocation();
		// Check distance from player to mouse
		float distFromPlayerToMouse = FVector::Dist(playerPos, mousePos);

		// Initialize the finalPos
		FVector finalPos;
		// If the dist is less than the MaxCastRange
		if (distFromPlayerToMouse < AbilityE.MaxCastRange) {
			finalPos = mousePos;
		}
		else {
			// Make it so the AimingCapsule cant go beyond the MaxCastRange
			FVector vectorFromPlayerToMouse = FVector(mousePos - playerPos);
			vectorFromPlayerToMouse.Normalize();
			finalPos = playerPos + vectorFromPlayerToMouse * AbilityE.MaxCastRange;
		}

		// Make sure the AimingCapsule is not lower in the world than the player.
		// This prevents the player from spawning inthe ground and other issues.
		if (finalPos.Z < playerPos.Z) finalPos.Z = playerPos.Z;

		// Set the VD_E_AimingCapsule to the finalPos
		SetCurrentCapsuleLocation(finalPos);
		player->VD_E_AimingCapsule->SetWorldLocation(finalPos);
	}
		break;
	case EAbilityID::ABID_R:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using Dagger: R");
		break;
	default:
		break;
	}
}


/**********************************************************/
// 
/**********************************************************/
//*************** ABILITY SPECIFIC METHODS ***************//
void UKOA_Artifact_DualDaggers::E_LocationSwap() {
	// Set the player to the mouse position
	AKOA_PROTO_Character* player = GetPlayerReference();
	player->SetActorLocation(GetCurrentECapsuleLocation());
}