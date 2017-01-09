// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_PROTO_CharacterMovementSlide.h"
#include "KOA_PROTO_Character.h"

void UKOA_PROTO_CharacterMovementSlide::InitializeComponent() {
	Super::InitializeComponent();
}

//Tick Comp
void UKOA_PROTO_CharacterMovementSlide::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->MovementMode.GetValue() == MOVE_Custom && this->CustomMovementMode == (uint8)ECustomMovementType::CMT_WallSlide)
	{
		// Get Player location
		FVector playerLocation = GetActorLocation();
		// Get reference to the player character.
		AKOA_PROTO_Character* pc = Cast<AKOA_PROTO_Character>(this->GetCharacterOwner());
		// Set jump stats
		pc->JumpStats.ApplyWallSlideAcceleration(DeltaTime);
		pc->JumpStats.DisplayWallSlideDebugInfo();
		// Define a movement vector
		FVector vMovement = -(pc->JumpStats.GetCurrSlideVelocity() * pc->GetActorUpVector());
		FHitResult hitResult;
		// Zero out the initial Velocity
		Velocity = FVector::ZeroVector;
		// Issue movement command to slide down the wall
		SafeMoveUpdatedComponent(vMovement * DeltaTime, pc->GetActorRotation(), true, hitResult);
	}
}

