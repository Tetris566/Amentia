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

	static ConstructorHelpers::FObjectFinder<UClass> Pillar(TEXT("Class'/Game/Artifacts/MatterHammer/Abilities/MH_PillarBP.MH_PillarBP_C'"));
	MH_Pill = Pillar.Object;

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
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed MatterHammer::Q");
}

void UKOA_Artifact_MatterHammer::PressAbilityW() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed MatterHammer::W");
}

void UKOA_Artifact_MatterHammer::PressAbilityE() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed MatterHammer::E");
	//Spawn projectile now.
	//GetPlayerReference()->GetWorld()->SpawnActor(MH_Plat, &PlatPos);
}

void UKOA_Artifact_MatterHammer::PressAbilityR() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You pressed MatterHammer::R");
}


//********** RELEASE ABILITY **********//

void UKOA_Artifact_MatterHammer::ReleaseAbilityQ() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released MatterHammer::Q");

	//Spawn Object...
	GetPlayerReference()->GetWorld()->SpawnActor(MH_Plat, &PlatPos);

	AbilityQ.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_Q);
}

void UKOA_Artifact_MatterHammer::ReleaseAbilityW() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released MatterHammer::W");
	
	//Spawn Object...
	GetPlayerReference()->GetWorld()->SpawnActor(MH_Pill, &PillPos);

	AbilityW.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_W);
}

void UKOA_Artifact_MatterHammer::ReleaseAbilityE() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released MatterHammer::E");
	AbilityE.SetAbilityOnCooldown();
	StartAbilityCooldownTimer(EAbilityID::ABID_E);
}

void UKOA_Artifact_MatterHammer::ReleaseAbilityR() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, "CODE: You released MatterHammer::R");
	AbilityR.SetAbilityOnCooldown();
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
			PlatPos = mousePos;
		}
		else {
			// Make it so the platform can't go beyond the MaxCastRange
			FVector vectorFromPlayerToMouse = FVector(mousePos - playerPos);
			vectorFromPlayerToMouse.Normalize();
			finalPos = playerPos + vectorFromPlayerToMouse * AbilityQ.MaxCastRange;

			PlatPos = finalPos;
		}
	}
		//Spawn Object when released.
		break;
	case EAbilityID::ABID_W:
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Aiming Pillar...");
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
		FVector LineEnd;
		FVector finalRot;
		FActorSpawnParameters SpawnInfo;

		// If the dist is less than the MaxCastRange
		if (distFromPlayerToMouse < AbilityW.MaxCastRange) {

			finalPos = mousePos;
			LineEnd = FVector(finalPos.X, finalPos.Y, finalPos.Z - 500);

			FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true);
			RV_TraceParams.bTraceComplex = true;
			RV_TraceParams.bTraceAsyncScene = true;
			RV_TraceParams.bReturnPhysicalMaterial = false;

			//Re-initialize hit info
			FHitResult RV_Hit(ForceInit);

			//call GetWorld() from within an actor extending class
			GetPlayerReference()->GetWorld()->LineTraceSingleByChannel(
				RV_Hit,        //result
				finalPos,      //start
				LineEnd,       //end
				ECC_Pawn,      //collision channel
				RV_TraceParams
				);

			if (RV_Hit.bBlockingHit) {
				PillPos = RV_Hit.ImpactPoint;
			}
		}
		else {
			// Make it so the platform can't go beyond the MaxCastRange
			FVector vectorFromPlayerToMouse = FVector(mousePos - playerPos);
			vectorFromPlayerToMouse.Normalize();
			finalPos = playerPos + vectorFromPlayerToMouse * AbilityW.MaxCastRange;
			LineEnd = FVector(finalPos.X, finalPos.Y, finalPos.Z - 500);

			//TODO: Attach to floor...
			
			FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true);
			RV_TraceParams.bTraceComplex = true;
			RV_TraceParams.bTraceAsyncScene = true;
			RV_TraceParams.bReturnPhysicalMaterial = false;

			//Re-initialize hit info
			FHitResult RV_Hit(ForceInit);

			//call GetWorld() from within an actor extending class
			GetPlayerReference()->GetWorld()->LineTraceSingleByChannel(
				RV_Hit,        //result
				finalPos,      //start
				LineEnd,       //end
				ECC_Pawn,      //collision channel
				RV_TraceParams
			);

			if (RV_Hit.bBlockingHit) {
				PillPos = RV_Hit.ImpactPoint;
			}

			//PillPos = finalPos;
		}
	}
		//Spawn Pillar when released.
		break;
	case EAbilityID::ABID_E:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Holding MH:E...");
		break;
	break;
	case EAbilityID::ABID_R:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, "Holding MH:R...");
		break;
	default:
		break;
	}
}



