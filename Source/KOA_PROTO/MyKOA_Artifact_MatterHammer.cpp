// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "MyKOA_Artifact_MatterHammer.h"
#include "KOA_PROTO_Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UTIL_MouseFunctionality.h"

UKOA_Artifact_MatterHammer::UKOA_Artifact_MatterHammer(const FObjectInitializer& ObjectInitializer)
	: Super() {
	ArtifactID = EArtifactID::ID_MatterHammer;
	ArtifactName = "It Doesn't Matter Hammer";
	LightBasicAttackLockDuration = 1.0f;

	

	static ConstructorHelpers::FObjectFinder<UClass> Platform(TEXT("Class'/Game/Artifacts/MatterHammer/Abilities/MH_PlatformBP.MH_PlatformBP_C'"));
	MH_Plat = Platform.Object;

	// ABILITY Q //
	AbilityQ.AbilityName = "Platform";
	AbilityQ.AbilityOnCooldown = false;

	// ABILITY W //
	AbilityW.AbilityName = "Pillar";
	AbilityW.AbilityOnCooldown = false;

	// ABILITY E //
	AbilityE.AbilityName = "Slow Ball";
	AbilityE.AbilityOnCooldown = false;

	// ABILITY R //
}

//********** USE BASIC ATTACKS **********//
void UKOA_Artifact_MatterHammer::UseLightAttack() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed MatterHammer::LightAttack");
	Super::UseLightAttack();
}

//********** PRESS ABILITY **********//
void UKOA_Artifact_MatterHammer::PressAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "You pressed MatterHammer::Q");
}

void UKOA_Artifact_MatterHammer::PressAbilityW() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed MatterHammer::W");
	AbilityW.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_W);
}

void UKOA_Artifact_MatterHammer::PressAbilityE() {
	AKOA_PROTO_Character* player = GetPlayerReference();
	player->SetIsMovementInputDisabled(true);
}

void UKOA_Artifact_MatterHammer::PressAbilityR() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed MatterHammer::R");
}


//********** RELEASE ABILITY **********//

void UKOA_Artifact_MatterHammer::ReleaseAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released MatterHammer::Q");

	//Spawn Object...
	GetPlayerReference()->GetWorld()->SpawnActor(MH_Plat, &TempPos);

	AbilityQ.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_Q);
}

void UKOA_Artifact_MatterHammer::ReleaseAbilityW() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released MatterHammer::W");
	//StartAbilityCooldownTimer(EAbilityID::ABID_W);
}

void UKOA_Artifact_MatterHammer::ReleaseAbilityE() {
	// Set the aiming mesh component visibility to false
	AKOA_PROTO_Character* player = GetPlayerReference();
	player->SetIsMovementInputDisabled(false);
	StartAbilityCooldownTimer(EAbilityID::ABID_E);
}

void UKOA_Artifact_MatterHammer::ReleaseAbilityR() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released MatterHammer::R");
	StartAbilityCooldownTimer(EAbilityID::ABID_R);
}

//******************** TICK ********************//
void UKOA_Artifact_MatterHammer::Tick(float DeltaTime) {

	switch (GetCurrentHeldAbilityButton()) {
	case EAbilityID::ABID_Q:
	{

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Aiming Platform...");
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
		FVector finalRot;
		FActorSpawnParameters SpawnInfo;

		// If the dist is less than the MaxCastRange
		if (distFromPlayerToMouse < AbilityQ.MaxCastRange) {
			TempPos = mousePos;
		}
		else {
			// Make it so the platform can't go beyond the MaxCastRange
			FVector vectorFromPlayerToMouse = FVector(mousePos - playerPos);
			vectorFromPlayerToMouse.Normalize();
			finalPos = playerPos + vectorFromPlayerToMouse * AbilityQ.MaxCastRange;

			TempPos = finalPos;
		}
	}
		//Spawn Object when released.
		break;
	case EAbilityID::ABID_W:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Aiming Pillar...");
		break;
	case EAbilityID::ABID_E:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using MatterHammer: E...");
		break;
	break;
	case EAbilityID::ABID_R:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Using MatterHammer: R...");
		break;
	default:
		break;
	}
}



