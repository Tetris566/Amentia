// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_Artifact_FireGlove.h"
#include "KOA_PROTO_Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UTIL_MouseFunctionality.h"

UKOA_Artifact_FireGlove::UKOA_Artifact_FireGlove() 
	: Super() {
	ArtifactID = EArtifactID::ID_FireGlove;  
	ArtifactName = "Flower Glove";

	AbilityQ.AbilityName = "Kamehame-FI-ha Ball";
	AbilityQ.AbilityOnCooldown = false;

	AbilityW.AbilityName = "W";
	AbilityW.AbilityOnCooldown = false;

	AbilityE.AbilityName = "E";
	AbilityE.AbilityOnCooldown = false;


}

void UKOA_Artifact_FireGlove::UseLightAttack() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed FireGlove::LightAttack");
	Super::UseLightAttack();
}

void UKOA_Artifact_FireGlove::PressAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "You pressed FireGlove::Q");
	AbilityQ.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_Q);
}

void UKOA_Artifact_FireGlove::PressAbilityW() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "You pressed FireGlove::W");
	AbilityW.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_W);
}

void UKOA_Artifact_FireGlove::PressAbilityE() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "You pressed FireGlove::E");
	AbilityE.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_E);
}