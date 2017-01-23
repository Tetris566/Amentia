// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_PROTO_Wall.h"
#include "KOA_PROTO_Character.h"
#include "KOA_PROTO_CharacterMovementSlide.h"
#include "KOA_BASE_Artifact.h"
#include "MyKOA_Artifact_MatterHammer.h"
#include "UTIL_MouseFunctionality.h"

//////////////////////////////////////////////////////////////
// 				CONSTRUCTORS AND INITIALIZERS 				//
//////////////////////////////////////////////////////////////
// Default Constructor
AKOA_PROTO_Character::AKOA_PROTO_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKOA_PROTO_CharacterMovementSlide>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values
	PlayerName = "Chester";

	// Stats
	HealthCurrent = 75.0f;
	HealthMax = 100.0f;

	// Movement
	WalkSpeed = 450.0f;
	RunSpeed = 900.0f;
	CanDodge = false;
	IsMovementInputDisabled = false;
	//JumpStats.EnableDoubleJumping();
	IsSlidingDownWall = false;
	
	// Inventory //
	IsAimingThrowable = false;
	CurrentThrowable = ETYPEOF_Throwable::NONE;
	
	SE_BLEED_MaxAmount = 120.0f;
	SE_BURN_MaxAmount = 15.0f;
	SE_POISON_MaxAmount = 25.0f;
	
	// Initialize Current Artifact
	CurrentArtifact = EArtifactID::ID_NULL;

	// Initialize ability and artifact locks
	IsAbilityUseLocked = false;
	IsArtifactSwapLocked = false;
	ArtifactSwapLockDuration = 1.0f;
	AbilityLockDuration = 0.5f;
	AbilityPressed = EAbilityID::NONE;

	// Get a pointer to the current world
	WorldPtr = GetWorld();

	// Create and attach the E_AimingMesh to it's CapsuleComponent
	VD_E_AimingCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("VD_E_AimingCapsule"));
	
	VD_E_AimingMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VD_E_AimingMeshComponent"));
	VD_E_AimingMeshComponent->AttachTo(VD_E_AimingCapsule);
	
}

// Called when the game starts or when spawned
void AKOA_PROTO_Character::BeginPlay() {
	Super::BeginPlay();	
}
// Clears up things like timers and variables
void AKOA_PROTO_Character::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	for (int n = 0; n < CollectedArtifacts.Num(); ++n) {
		UKOA_BASE_Artifact* artifact = CollectedArtifacts[n]->GetDefaultObject<UKOA_BASE_Artifact>();
		
		// Reset Attack Cooldowns
		artifact->UninitializeBasicAttacks();
		
		// Reset Ability Cooldowns
		artifact->AbilityQ.ResetAbilityCooldown();
		artifact->AbilityW.ResetAbilityCooldown();
		artifact->AbilityE.ResetAbilityCooldown();
		artifact->AbilityR.ResetAbilityCooldown();
		
		// Reset Timer Handles
		FAbilityTimerHandles TimerHandles = artifact->GetArtifactAbilityTimerHandles();
		GetWorldPtr()->GetTimerManager().ClearTimer(TimerHandles.LightAttackTimer);
		GetWorldPtr()->GetTimerManager().ClearTimer(TimerHandles.AbilityQTimer);
		GetWorldPtr()->GetTimerManager().ClearTimer(TimerHandles.AbilityWTimer);
		GetWorldPtr()->GetTimerManager().ClearTimer(TimerHandles.AbilityETimer);
		GetWorldPtr()->GetTimerManager().ClearTimer(TimerHandles.AbilityRTimer);	
		
		// Enable Movement input
		SetIsMovementInputDisabled(false);
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AKOA_PROTO_Character::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	
	// if (AbilityPressed != EAbilityID::NONE) {
	// 	// Access the current artifact's tick function
	// 	CurrentArtifact.Tick(DeltaTime);
	// }

	// Check to see if an ability button is being held
	if (AbilityPressed != EAbilityID::NONE) {
		//Access the current artifact's tick function
		UKOA_BASE_Artifact* currArtifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
		currArtifact->Tick(DeltaTime);
		//GetWorldPtr()->GetFirstPlayerController()->CurrentMouseCursor = (TEnumAsByte<EMouseCursor::Type>)EMouseCursor::Crosshairs;

			//TEnumAsByte<EMouseCursor::Type> CurrentMouseCursor;
	} else {
		//GetWorldPtr()->GetFirstPlayerController()->CurrentMouseCursor = (TEnumAsByte<EMouseCursor::Type>)EMouseCursor::Default;
	}


	// If we are sliding down the wall
	/*if (IsSlidingDownWall) {
		FVector playerLocation = GetActorLocation();
		JumpStats.ApplyWallSlideAcceleration(DeltaTime);
		JumpStats.DisplayWallSlideDebugInfo();
		// playerLocation - (distance * -UpVector * DeltaTime)
		FVector finalLocation = playerLocation - (JumpStats.GetCurrSlideVelocity() * GetActorUpVector() * DeltaTime);
		//TODO: Implement a better floor detection
		if (finalLocation.Z < 110.15) finalLocation.Z = 110.15;
		SetActorLocation(finalLocation);
	}*/
}

// Called to bind functionality to input
void AKOA_PROTO_Character::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent);

	/***** AXIS BINDINGS *****/
	InputComponent->BindAxis("MoveRight", this, &AKOA_PROTO_Character::MoveRight);

	/***** ACTION BINDINGS *****/
	// Movement Bindings //
	InputComponent->BindAction("Run", IE_Pressed, this, &AKOA_PROTO_Character::SetMoveSpeedToRun);
	InputComponent->BindAction("Run", IE_Released, this, &AKOA_PROTO_Character::SetMoveSpeedToWalk);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AKOA_PROTO_Character::PlayerJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AKOA_PROTO_Character::PlayerStopJump);
	// Inventory //
	InputComponent->BindAction("OpenInventory", IE_Pressed, this, &AKOA_PROTO_Character::OpenInventory);
	// Throwables //
	InputComponent->BindAction("ThrowThrowable", IE_Pressed, this, &AKOA_PROTO_Character::AimCurrentThrowable);
	InputComponent->BindAction("ThrowThrowable", IE_Released, this, &AKOA_PROTO_Character::ThrowCurrentThrowable);
	// Ability Bindings //
	// Q //
	InputComponent->BindAction("AbilityQ", IE_Pressed, this, &AKOA_PROTO_Character::PressCurrentAbilityQ);
	InputComponent->BindAction("AbilityQ", IE_Released, this, &AKOA_PROTO_Character::ReleaseCurrentAbilityQ);
	// W //
	InputComponent->BindAction("AbilityW", IE_Pressed, this, &AKOA_PROTO_Character::PressCurrentAbilityW);
	InputComponent->BindAction("AbilityW", IE_Released, this, &AKOA_PROTO_Character::ReleaseCurrentAbilityW);
	// E //
	InputComponent->BindAction("AbilityE", IE_Pressed, this, &AKOA_PROTO_Character::PressCurrentAbilityE);
	InputComponent->BindAction("AbilityE", IE_Released, this, &AKOA_PROTO_Character::ReleaseCurrentAbilityE);
	// R //
	InputComponent->BindAction("AbilityR", IE_Pressed, this, &AKOA_PROTO_Character::PressCurrentAbilityR);
	InputComponent->BindAction("AbilityR", IE_Released, this, &AKOA_PROTO_Character::ReleaseCurrentAbilityR);
	// Artifact Bindings //
	InputComponent->BindAction("EquipArtifact_DualDaggers",  IE_Pressed, this, &AKOA_PROTO_Character::EquipDualDaggers);
	InputComponent->BindAction("EquipArtifact_FireGlove",    IE_Pressed, this, &AKOA_PROTO_Character::EquipFireGlove);
	InputComponent->BindAction("EquipArtifact_MatterHammer", IE_Pressed, this, &AKOA_PROTO_Character::EquipMatterHammer);
	// Basic Attacks //
	InputComponent->BindAction("LightAttack",IE_Pressed, this, &AKOA_PROTO_Character::UseCurrBasicAttackLight);
	//TODO: QuickArtifactSelect press release
}
//////////////////////////////////////////////////////////////
// 						  UTILITY 							//
//////////////////////////////////////////////////////////////
FVector AKOA_PROTO_Character::GetMousePositionInPlayerPlane() {
	return UTIL_MouseFunctionality::GetMousePosInPlayerPlane(this->GetWorldPtr());
}
//////////////////////////////////////////////////////////////
// 						PLAYER STATS 						//
//				Methods to manage player stats.				//
//////////////////////////////////////////////////////////////
void AKOA_PROTO_Character::DamagePlayer(float Amount) {
	(HealthCurrent - Amount) < 0.0f ? HealthCurrent = 0.0f : HealthCurrent -= Amount;
	//TODO: Tell the player he is dead if HealthCurrent <= 0.0f
}
void AKOA_PROTO_Character::HealPlayer(float Amount) {
	(HealthCurrent + Amount) > HealthMax ? HealthCurrent = HealthMax : HealthCurrent += Amount;
}

//////////////////////////////////////////////////////////////
// 						   MOVEMENT 						//
//				Methods to handle movement.					//
//				Walking, Running, Jumping, Sliding			//
//////////////////////////////////////////////////////////////
void AKOA_PROTO_Character::SetMoveSpeedToRun() {
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}
void AKOA_PROTO_Character::SetMoveSpeedToWalk() {
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// MoveRight(Amount): 
//	 Move right if Amount > 0; left if Amount is < 0
void AKOA_PROTO_Character::MoveRight(float Amount) {
	// Only move if the controller is set up and Amount is not 0
	if (Controller && Amount && GetIsMovementInputDisabled() == false) {
		// AddMovementInput in the direction of the right vector
		AddMovementInput(FVector(0.0, 1.0, 0.0), Amount);
	}
}

bool AKOA_PROTO_Character::GetIsMovementInputDisabled() const {
	return IsMovementInputDisabled;
}
void AKOA_PROTO_Character::SetIsMovementInputDisabled(bool IsDisabled) {
	IsMovementInputDisabled = IsDisabled;
}

//////////////////////////////////////////////////////////////
// 						   JUMPING 							//
//				Methods to handle jumping logic.			//
//////////////////////////////////////////////////////////////
// DetectWall():
//		Used to detect if there is a wall the player can jump off of.
//		If there is, it returns info detailing the wall's properties.
FDetectWallHitInfo AKOA_PROTO_Character::DetectWall() {
	// Create hitinfo
	FDetectWallHitInfo DWHitInfo;

	// Create a local variable to keep track of displaying debug info
	EDrawDebugTrace::Type DebugDuration;
	(JumpStats.DebugInfo) ? (DebugDuration = EDrawDebugTrace::ForDuration) : (DebugDuration = EDrawDebugTrace::None);

	// Create local varialbes to be used in SphereTrace
	// Start at player location
	FVector PlayerLocation = GetActorLocation();
	// from there, go up the players height to the end
	FVector EndLocation = PlayerLocation + (100 * GetActorUpVector());
	// radius of the sphere to trace
	float Radius = 50;
	// Array of object to hit; Need to figure out exactly how to set this up
	// I BELIEVE ObjectTypeQuery1 is WorldStatic and ObjectTypeQuery2 is WorldDynamic
	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);

	// Actors to ignore
	TArray<AActor*>ActorsToIgnore;
	// Hit result
	FHitResult HitResult;

	// Perform the sphere trace 
	bool HitWall = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, PlayerLocation, EndLocation, Radius, ObjectTypes,
		false, ActorsToIgnore, DebugDuration, HitResult, true);

	// If you hit a wall 
	if (HitWall) {
		// Do another sphere trace, at the players feet in the direction they are facing
		EndLocation = PlayerLocation + (25 * GetActorForwardVector());
		Radius = 25;
		bool CanJump = UKismetSystemLibrary::SphereTraceSingleForObjects(
			this, PlayerLocation, EndLocation, Radius, ObjectTypes,
			false, ActorsToIgnore, DebugDuration, HitResult, true);

		// If you CanJump
		if (CanJump) {
			DWHitInfo.SetCanJump(true);
			FVector forward = GetActorForwardVector();
			FVector right = FVector(0.0f, 1.0f, 0.0f);

			// Take the dot product with the forward vector and the right world vector
			// only need the y and z, x never moves
			float dot = forward.Y * right.Y + forward.Z * right.Z;
			// if negative, wall is to the left, if positive it is to the right
			(dot < 0) ? DWHitInfo.SetWallDirection(-1) : DWHitInfo.SetWallDirection(1);

			// Check if the actor you hit is actually a wall we can extract information from.
			AKOA_PROTO_Wall* wall = Cast<AKOA_PROTO_Wall>(HitResult.GetActor());
			if (wall) {
				DWHitInfo.SetWallHitInfo(wall->GetWallInfo());
			}
			else {
				
				//TODO: Make sure wall slide works with this disabled.
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, "ERROR: Hitting NON-Wall.");
				//DWHitInfo.SetWallHitInfo(AKOA_PROTO_Wall::SmoothWallInfo);
			}
		}
	}
	// If you didn't hit a wall, all the values will be the default values; otherwise return the proper results
	return DWHitInfo;
}

// PlayerJump():
//		Main method involded in the player jump logic.
void AKOA_PROTO_Character::PlayerJump() {
	if (GetIsMovementInputDisabled() == false) {
		// If the player is grounded, then this is their first jump
		if (!GetCharacterMovement()->IsFalling()) {
			ACharacter::Jump();
		}
		// The player has already jumped or is otherwise in the air
		else {
			// Check to see if we can jump off a wall
			FDetectWallHitInfo WallHitInfo = DetectWall();

			// If you can jump off a wall do it
			if (WallHitInfo.GetCanJump()) {
				// Disable movement to allow user to hang on wall
				//GetCharacterMovement()->DisableMovement();

				//Set movement mode to WallSlide
				GetCharacterMovement()->SetMovementMode(MOVE_Custom, (uint8)ECustomMovementType::CMT_WallSlide);
				// Pass the results up to the player
				JumpStats.SetHangingOnWall(true);
				JumpStats.SetWallOnPlayerSide(WallHitInfo.GetWallDirection());
				// Set the stats that you got from the wall hit
				JumpStats.SetCurrSlideVelocity(WallHitInfo.WallHitSlideVelocity);
				JumpStats.SetSlideAcceleration(WallHitInfo.WallHitSlideAcceration);
				JumpStats.SetWallHoldDuration(WallHitInfo.WallHitHoldDuration);
				// Set the timer for holding onto the wall
				StartWallHoldTimer(JumpStats.GetWallHoldDuration());

			} // Otherwise, check if you can double jump, if so do it,
			else if (JumpStats.GetCanDoubleJump()) {
				JumpStats.SetCanDoubleJump(false);
				ACharacter::Jump();
			}
		}
	}
}

// Using the unit circle, 60deg is at ((+/-)1/2, sqrt(3)/2)
// Use this for the launch angle of the player off of the wall
const float ROOT_TWO_OVER_TWO   = 0.70710678118;
const float ROOT_THREE_OVER_TWO = 0.86602540378;
void AKOA_PROTO_Character::PlayerStopJump() {
	// If you are hanging on the wall, jump off
	if (JumpStats.GetHangingOnWall()) {
		// Multiply JumpStats.GetWallOnPlayerSide into the y value for it to launch in the proper direction
		GetCharacterMovement()->AddImpulse(
			FVector( 
				0.0f,
				-1 * JumpStats.GetWallOnPlayerSide() * JumpStats.Power * 0.5, // -1 is for making sure you apply jump in the proper direction, inverts GetWallOnPlayerSide()
				JumpStats.Power * ROOT_THREE_OVER_TWO
			),	
			true
		);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		JumpStats.SetHangingOnWall(false);
		IsSlidingDownWall = false;
		ClearWallHoldTimer();
		ClearWallSlideTimer();
	}
	ACharacter::StopJumping();
}

//Landed(const FHitResult &Hit): 
//		Override of ACharacter::Landed(const FHitResult &Hit)
//		Used in jump logic
void AKOA_PROTO_Character::Landed(const FHitResult &Hit) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Player has landed.");
	Super::Landed(Hit);

	// Not sliding on a wall
	IsSlidingDownWall = false;
	// Reset JumpStats
	JumpStats.SetHangingOnWall(false);
	JumpStats.SetWallOnPlayerSide(0);
	// If you can double jump, reset the jump bool.
	if (JumpStats.IsDoubleJumpEnabled()) JumpStats.SetCanDoubleJump(true);
}

void AKOA_PROTO_Character::StartWallSlide() {
	ClearWallHoldTimer();
	// Used in the Tick() event to "Slide" the player down the wall
	IsSlidingDownWall = true;
	// Begin the timer for sliding down the wall
	StartWallSlideTimer(2.0f);
}

void AKOA_PROTO_Character::LoseGripAndFall() {
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	JumpStats.SetHangingOnWall(false);
	IsSlidingDownWall = false;
	// Clear the wall slide timer
	ClearWallSlideTimer();
}
//////////////////////////////////////////////////////////////
// 						 INVENTORY 							//
//				Methods to handle inventory 				//
//////////////////////////////////////////////////////////////

//void AKOA_PROTO_Character::OpenInventory() 
UAMTA_BASE_Throwable* AKOA_PROTO_Character::GetCurrThrowableRefernce() const {
	UAMTA_BASE_Throwable* ptr = nullptr;
	if (CurrentThrowable != ETYPEOF_Throwable::NONE) {
		ptr = CollectedThrowables[(uint8)CurrentThrowable]->GetDefaultObject<UAMTA_BASE_Throwable>();
	}
	return ptr;
}
//void AKOA_PROTO_Character::ThrowCurrentThrowable() {}
//////////////////////////////////////////////////////////////
// 					  STATUS EFFECTS 						//
//		Methods to handle dealing with status effects 		//
//////////////////////////////////////////////////////////////
//************************ BLEED ***************************//
void AKOA_PROTO_Character::ApplyBLEEDBuildUp(float Amount){
	if (SE_BLEED_BuildUp + Amount >= SE_BLEED_MaxAmount) {
		ApplyBLEED();
	} else {
		SE_BLEED_BuildUp += Amount;
	}
}
void AKOA_PROTO_Character::ApplyBLEED() {
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.7f, FColor::Red, "ApplyBLWEED");
	SE_BLEED_BuildUp = 0.0;
	IsBLEEDING = true;
}
//************************* BURN ***************************//	
void AKOA_PROTO_Character::ApplyBURNBuildUp(float Amount) {
	if (SE_BURN_BuildUp + Amount >= SE_BURN_MaxAmount) {
		ApplyBURN();
	} else {
		SE_BURN_BuildUp += Amount;
	}
}
void AKOA_PROTO_Character::ApplyBURN() {
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.7f, FColor::Red, "ApplyBURN");
	SE_BURN_BuildUp = 0.0;
	IsBURNING = true;
}
//************************* POISON ***************************//
void AKOA_PROTO_Character::ApplyPOISONBuildUp(float Amount) {
	if (SE_POISON_BuildUp + Amount >= SE_POISON_MaxAmount) {
		ApplyPOISON();
	} else {
		SE_POISON_BuildUp += Amount;
	}
}
void AKOA_PROTO_Character::ApplyPOISON() {
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.7f, FColor::Red, "ApplyPOISON");
	SE_POISON_BuildUp = 0.0;
	IsPOISONED = true;
}
//////////////////////////////////////////////////////////////
// 						  ARTIFACTS 						//
//				Methods to handle artifacts 				//
//////////////////////////////////////////////////////////////
// EquipDualDaggers():
//		Wrapper method used for binding DualDaggers to a button for gameplay / debug.
void AKOA_PROTO_Character::EquipDualDaggers() {
	// if you are equipping daggers that aren't already equipped
	if (GetIsArtifactSwapLocked() != true && SetCurrentArtifact(EArtifactID::ID_DualDaggers)) {
		DEBUG_EquipCurrentArtifact();
		// Lock artifact swaping
		IsArtifactSwapLocked = true;
		// Lock ability use
		//IsAbilityUseLocked = true;
		StartArtifactSwapLockTimer(ArtifactSwapLockDuration);
	}
}
// EquipFireGlove():
//		Same as DualDaggers, except with the FireGlove...
void AKOA_PROTO_Character::EquipFireGlove() {
	
	if (!GetIsArtifactSwapLocked() && SetCurrentArtifact(EArtifactID::ID_FireGlove)) {
		DEBUG_EquipCurrentArtifact();
		// Lock artifact swaping
		IsArtifactSwapLocked = true;
		// Lock ability use
		//IsAbilityUseLocked = true;
		StartArtifactSwapLockTimer(ArtifactSwapLockDuration);
	}
}

void AKOA_PROTO_Character::EquipMatterHammer() {

	if (!GetIsArtifactSwapLocked() && SetCurrentArtifact(EArtifactID::ID_MatterHammer)) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.7f, FColor::Red, "Hammer Success");
		DEBUG_EquipCurrentArtifact();
		// Lock artifact swaping
		IsArtifactSwapLocked = true;
		// Lock ability use
		//IsAbilityUseLocked = true;
		StartArtifactSwapLockTimer(ArtifactSwapLockDuration);
	}
	else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.7f, FColor::Red, "Hammer Fail");
	}
}

/*


// Switch on the ability being used
switch (AbilityID) {
case EAbilityID::ABID_Q:
// If Q isn't on cooldown...
if (artifact->AbilityQ.IsAbilityOnCooldown() == false) {
// Lock ability use until you release the button
IsAbilityUseLocked = true;
SetWhichAbilityPressed(EAbilityID::ABID_Q);
// Run the abilityQ press on current artifact
artifact->SetCurrentHeldAbilityButton(EAbilityID::ABID_Q);
artifact->PressAbilityQ();
}

*/

void AKOA_PROTO_Character::UseCurrBasicAttackLight() {
	// Make sure artifact equipped
	if (CurrentArtifact != EArtifactID::ID_NULL) {
		// Get the current artifact
		UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
		// If basic attacks aren't on cooldown
		if (artifact->GetIsBasicAttackOnCooldown() == false) {
			SetIsMovementInputDisabled(true);
			artifact->UseLightAttack();
		} 
	} else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f,FColor::Red, "ERROR: No CurrentArtifact for Light Attack.");
	}
}

bool AKOA_PROTO_Character::GetIsArtifactSwapLocked() const {
	return IsArtifactSwapLocked;
}

bool AKOA_PROTO_Character::GetAbilitiesOnCooldown() const {
	//Set true if any abilities are on CD.
	if (CurrentArtifact != EArtifactID::ID_NULL) {
		// Get the current artifact
		UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
		if (artifact->GetIsBasicAttackOnCooldown()) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ERROR: No CurrentArtifact for Light Attack.");
		return false;
	}
}

UKOA_BASE_Artifact* AKOA_PROTO_Character::GetCurrArtifactReference() const {
	UKOA_BASE_Artifact* ptr = nullptr;
	if (CurrentArtifact != EArtifactID::ID_NULL) {
		ptr = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
	}
	return ptr;
}

EArtifactID AKOA_PROTO_Character::GetEquippedArtifact() const {
	return this->CurrentArtifact;
}

// SetCurrentArtifact(Artifact): 
//		Sets the player's CurrentArtifact ENUM to Artifact. Checks to see if
//		the player has collected the artifact.
//		RETURNS: 
//			true: if you are setting CurrentArtifact to the value of Artifact for the first time.
//			false: if you either don't have it or it is already equipped
bool AKOA_PROTO_Character::SetCurrentArtifact(EArtifactID Artifact) {
	//TODO: Further/Better explain: CollectedArtifacts.Num() > (uint8)Artifact
	// If you have collected the artifact at this point, you can equip it...
	// Also make sure you don't have it equipped

	//Where the fuck is Collected Artifacts being added to?

	if (CollectedArtifacts.Num() > (uint8)Artifact && Artifact != CurrentArtifact) {
		UKOA_BASE_Artifact* artifact = nullptr;
		artifact = CollectedArtifacts[(uint8)Artifact]->GetDefaultObject<UKOA_BASE_Artifact>();
		if (artifact != nullptr) {
			CurrentArtifact = Artifact;
			SetCurrArtifactPlayerReference();
			return true;
		}
	} 
	return false;
}

void AKOA_PROTO_Character::SetCurrArtifactPlayerReference() {
	UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
	artifact->SetPlayerReference(this);
}

void AKOA_PROTO_Character::UnlockArtifactSwap() {
	IsArtifactSwapLocked = false;
}

void AKOA_PROTO_Character::DEBUG_EquipCurrentArtifact() {
	switch (CurrentArtifact) {
	case EArtifactID::ID_DualDaggers:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Equipped: DualDaggers");
		break;

	case EArtifactID::ID_FireGlove:	
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Equipped: FireGlove");
		break;

	case EArtifactID::ID_MatterHammer:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Equipped: MatterHammer");
		break;

	case EArtifactID::ID_NULL:
	default:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ERROR: Either ID_NULL or INVALID EArtifactID");
		break;
	}
}


/**************************************************************************
	ABILITIES -
		Methods to handle using abilities
**************************************************************************/
////
/************************* PRESS CURRENT ABILITIES *************************/
////

void AKOA_PROTO_Character::PressCurrentAbility(EAbilityID AbilityID) {
	// Make sure the player has an Artifact equipped
	if (CurrentArtifact != EArtifactID::ID_NULL) {
		// If abilities aren't locked
		if (GetIsAbilityUseLocked() != true) {
			// Get the current artifact
			UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();

			// Switch on the ability being used
			switch (AbilityID) {
			case EAbilityID::ABID_Q: 
				// If Q isn't on cooldown...
				if (artifact->AbilityQ.IsAbilityOnCooldown() == false) {
					// Lock ability use until you release the button
					IsAbilityUseLocked = true;
					SetWhichAbilityPressed(EAbilityID::ABID_Q);
					// Run the abilityQ press on current artifact
					artifact->SetCurrentHeldAbilityButton(EAbilityID::ABID_Q);
					artifact->PressAbilityQ();
				}
				break;
			case EAbilityID::ABID_W:
				// If W isn't on cooldown...
				if (artifact->AbilityW.IsAbilityOnCooldown() == false) {
					// Lock ability use until you release the button
					IsAbilityUseLocked = true;
					SetWhichAbilityPressed(EAbilityID::ABID_W);
					// Run the abilityW press on current artifact
					artifact->SetCurrentHeldAbilityButton(EAbilityID::ABID_W);
					artifact->PressAbilityW();
				}
				break;
			case EAbilityID::ABID_E:
				// If E isn't on cooldown...
				if (artifact->AbilityE.IsAbilityOnCooldown() == false) {
					// Lock ability use until you release the button
					IsAbilityUseLocked = true;
					SetWhichAbilityPressed(EAbilityID::ABID_E);
					// Run the abilityE press on current artifact
					artifact->SetCurrentHeldAbilityButton(EAbilityID::ABID_E);
					artifact->PressAbilityE();
				}
				break;
			case EAbilityID::ABID_R:
				// If R isn't on cooldown...
				// TODO: CHECK IF STORM IS UNLOCKED
				if (artifact->IsArtifactStormUnlocked == true) {
					if (artifact->AbilityR.IsAbilityOnCooldown() == false) {
						// Lock ability use until you release the button
						IsAbilityUseLocked = true;
						SetWhichAbilityPressed(EAbilityID::ABID_R);
						// Run the abilityR press on current artifact
						artifact->SetCurrentHeldAbilityButton(EAbilityID::ABID_R);
						artifact->PressAbilityR();
					}
				}
				break;
			}
		}
	}
}
void AKOA_PROTO_Character::PressCurrentAbilityQ() {
	PressCurrentAbility(EAbilityID::ABID_Q);
}
void AKOA_PROTO_Character::PressCurrentAbilityW() {
	PressCurrentAbility(EAbilityID::ABID_W);
}
void AKOA_PROTO_Character::PressCurrentAbilityE() {
	PressCurrentAbility(EAbilityID::ABID_E);
}
void AKOA_PROTO_Character::PressCurrentAbilityR() {
	PressCurrentAbility(EAbilityID::ABID_R);
}

////
/************************* RELEASE CURRENT ABILITIES *************************/
////
void AKOA_PROTO_Character::ReleaseCurrentAbility(EAbilityID AbilityID) {
	// Make sure you have artifact equipped
	if (CurrentArtifact != EArtifactID::ID_NULL) {
		// Get the current artifact
		UKOA_BASE_Artifact* artifact = CollectedArtifacts[(uint8)CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
		switch (AbilityID) {
		case EAbilityID::ABID_Q:
			if (GetWhichAbilityPressed() == EAbilityID::ABID_Q) {
				if (artifact->AbilityQ.IsAbilityOnCooldown() == false) {
					artifact->AbilityQ.SetAbilityOnCooldown();
					artifact->ReleaseAbilityQ();
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				} else {
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				}
			}
			break;
		case EAbilityID::ABID_W:
			if (GetWhichAbilityPressed() == EAbilityID::ABID_W) {
				if (artifact->AbilityW.IsAbilityOnCooldown() == false) {
					artifact->AbilityW.SetAbilityOnCooldown();
					artifact->ReleaseAbilityW();
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				} else {
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				}
			}
			break;
		case EAbilityID::ABID_E:
			if (GetWhichAbilityPressed() == EAbilityID::ABID_E) {
				if (artifact->AbilityE.IsAbilityOnCooldown() == false) {
					artifact->AbilityE.SetAbilityOnCooldown();
					artifact->ReleaseAbilityE();
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				} else {
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				}
			}
			break;
		case EAbilityID::ABID_R:
			if (GetWhichAbilityPressed() == EAbilityID::ABID_R) {
				if (artifact->AbilityR.IsAbilityOnCooldown() == false) {
					artifact->AbilityR.SetAbilityOnCooldown();
					artifact->ReleaseAbilityR();
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				} else {
					AbilityPressed = EAbilityID::NONE;
					artifact->SetCurrentHeldAbilityButton(EAbilityID::NONE);
				}
			}
			break;
		}
	}
}
void AKOA_PROTO_Character::ReleaseCurrentAbilityQ() {
	ReleaseCurrentAbility(EAbilityID::ABID_Q);
}
void AKOA_PROTO_Character::ReleaseCurrentAbilityW() {
	ReleaseCurrentAbility(EAbilityID::ABID_W);
}
void AKOA_PROTO_Character::ReleaseCurrentAbilityE() {
	ReleaseCurrentAbility(EAbilityID::ABID_E);
}
void AKOA_PROTO_Character::ReleaseCurrentAbilityR() {
	ReleaseCurrentAbility(EAbilityID::ABID_R);
}

//****** GETTERS ******//
bool AKOA_PROTO_Character::GetIsAbilityUseLocked() const {
	return IsAbilityUseLocked;
}


EAbilityID AKOA_PROTO_Character::GetWhichAbilityPressed() const {
	return AbilityPressed;
}


bool AKOA_PROTO_Character::GetIsCurrentArtifactAbilityOnCooldown(const EAbilityID &AbilityID) const {
	// Get the current artifact see if it's Q is on cooldown
	UKOA_BASE_Artifact* artifact = this->CollectedArtifacts[(uint8)this->CurrentArtifact]->GetDefaultObject<UKOA_BASE_Artifact>();
	
	bool isOnCooldown = false;

	switch (AbilityID) {
	case EAbilityID::ABID_Q:
			isOnCooldown = artifact->AbilityQ.IsAbilityOnCooldown();
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "Current Ability Q IsOnCooldown: " + isOnCooldown);
			break;
	case EAbilityID::ABID_W:
		isOnCooldown = artifact->AbilityW.IsAbilityOnCooldown();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "Current Ability W IsOnCooldown: " + isOnCooldown);
		break;
	case EAbilityID::ABID_E:
		isOnCooldown = artifact->AbilityE.IsAbilityOnCooldown();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "Current Ability E IsOnCooldown: " + isOnCooldown);
		break;
	case EAbilityID::ABID_R:
		isOnCooldown = artifact->AbilityR.IsAbilityOnCooldown();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "Current Ability R IsOnCooldown: " + isOnCooldown);
		break;
	default:
		isOnCooldown = false;
		break;
	}
	
	return isOnCooldown;
}

//****** SETTERS ******//
void AKOA_PROTO_Character::SetIsAbilityUseLocked(bool Value) {
	IsAbilityUseLocked = Value;
}

void AKOA_PROTO_Character::SetWhichAbilityPressed(const EAbilityID& AbilityID) {
	AbilityPressed = AbilityID;
}

// UnlockAbilityUse():
//		Allows abilities to be used again.
void AKOA_PROTO_Character::UnlockAbilityUse() {
	IsAbilityUseLocked = false;
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, "UnlockAbilityUse()");
}

//void AKOA_PROTO_Character::ResetAbilityCooldown() {
//	//Ability.ResetAbilityCooldown();
//}

/**************************************************************************
	TIMERS -
		Methods for handling timers.
**************************************************************************/
void AKOA_PROTO_Character::StartAbilityLockTimer() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(AbilityLockTimer, this, &AKOA_PROTO_Character::UnlockAbilityUse, AbilityLockDuration, false);
	}
}


void AKOA_PROTO_Character::StartArtifactSwapLockTimer(const float &Duration) {
	// When the timer ends, unlock artifact swapping
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(ArtifactSwapLockTimer, this, &AKOA_PROTO_Character::UnlockArtifactSwap, Duration, false);
	}
}

void AKOA_PROTO_Character::StartWallHoldTimer(const float &Duration) {
	// When the timer ends, start sliding down the wall
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(WallHoldTimer, this, &AKOA_PROTO_Character::StartWallSlide, Duration, false);
	}
}

void AKOA_PROTO_Character::StartWallSlideTimer(const float &Duration) {
	// When the timer ends, lose your grip and fall
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(WallHoldTimer, this, &AKOA_PROTO_Character::LoseGripAndFall, Duration, false);
	}
}

void AKOA_PROTO_Character::ClearWallHoldTimer() {
	if (GetWorldPtr()) GetWorldPtr()->GetTimerManager().ClearTimer(WallHoldTimer);
}

void AKOA_PROTO_Character::ClearWallSlideTimer() {
	if (WorldPtr) WorldPtr->GetTimerManager().ClearTimer(WallSlideTimer);
}