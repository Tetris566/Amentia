// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "AMTA_BASE_Throwable.h"
#include "KOA_BASE_Artifact.h"
#include "KOA_PROTO_Wall.h"
#include "KOA_PROTO_Character.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementType : uint8 {
	CMT_WallSlide = 0	UMETA(DisplayName = "Wall Slide")
};

USTRUCT()
struct KOA_PROTO_API FDetectWallHitInfo {
	GENERATED_USTRUCT_BODY()

	// Display debug information duration
	bool CanJump;
	// Wall direction relative to player
	// -1: Left
	//  1: Right
	//  0: NULL
	int8 WallDirection;
	// WallHit infomation
	float WallHitSlideVelocity;
	float WallHitSlideAcceration;
	float WallHitHoldDuration;

	FDetectWallHitInfo() {
		CanJump = false;
		WallDirection = 0;
		WallHitSlideVelocity = 0.0f;
		WallHitSlideAcceration = 0.0f;
		WallHitHoldDuration = 0.0f;
	}

	void SetCanJump(const bool& value) {
		CanJump = value;
	}
	void SetWallDirection(const int8& value) {
		(value < 2 && value > -2) ? (WallDirection = value) : (WallDirection = 0);
	}
	void SetWallHitInfo(const FWallInformation& value) {
		WallHitSlideVelocity	= value.InitSlideVelocity;
		WallHitSlideAcceration	= value.SlideAcceration;
		WallHitHoldDuration		= value.WallHoldDuration;
	}

	FORCEINLINE bool GetCanJump() const {
		return CanJump;
	}
	FORCEINLINE int8 GetWallDirection() const {
		return WallDirection;
	}
};

// Structure for storing all player jump related variables and methods.
USTRUCT()
struct KOA_PROTO_API FPlayerJumpVariables {
	GENERATED_USTRUCT_BODY()

	// Display debug info
	UPROPERTY(EditAnywhere, Category = "Jump")
	bool DebugInfo;
	// The power of the jump
	UPROPERTY(EditAnywhere, Category = "Jump")
	float Power;

	//TODO: Move SlideAcceleration && SlideVelocity to private after init values are found
	//TODO:    and after demonstrating the feature.
	// The acceleration of the wall slide
	float SlideAcceleration;
	float CurrSlideVelocity;
	float MaxSlideVelocity;
	float FallOffWallHeight;

	/****** Constructor ******/
	FPlayerJumpVariables() {
		DebugInfo			= true;
		Power				= 1000.0f;
		HangingOnWall		= false;
		WallOnPlayerSide	= 0;
		CanDoubleJump		= false;
		DoubleJumpEnabled	= false;
		CurrSlideVelocity	= 1.0f; // Current Slide Velocity
		SlideAcceleration	= 98.0f; // Slide Acceleration
		MaxSlideVelocity	= 1000.0f;
		WallHoldDuration	= 0.0f;
		FallOffWallHeight	= 0.0f;
	}

	/****** Methods for FPlayerJumpVariables ******/
	void DisplayWallSlideDebugInfo() {
		if (DebugInfo) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Blue, TEXT("CurrSlideVelocity: " + FString::SanitizeFloat(CurrSlideVelocity)));
		}
	}
	void ApplyWallSlideAcceleration(const float& DeltaTime) {
		CurrSlideVelocity += (SlideAcceleration*DeltaTime);
		if (CurrSlideVelocity > MaxSlideVelocity) {
			CurrSlideVelocity = MaxSlideVelocity;
		}
	}
	void ResetWallSlideVariables() {
		CurrSlideVelocity = 1.0f;
	}

	/****** Setters for private variables ******/
	void SetHangingOnWall(const bool& value) {
		HangingOnWall = value;
	}
	void SetWallOnPlayerSide(const int8& value) {
		(value < 2 && value > -2) ? (WallOnPlayerSide = value) : (WallOnPlayerSide = 0);
	}
	// Only set CanDoubleJump == value if DoubleJumpEnabled == true, 
	// otherwise CanDoubleJump is ALWAYS false.
	void SetCanDoubleJump(const bool& value) {
		(DoubleJumpEnabled) ? (CanDoubleJump = value) : (CanDoubleJump = false);
	}
	void SetCurrSlideVelocity(const float& value) {
		(value < MaxSlideVelocity) ? (CurrSlideVelocity = value) : (CurrSlideVelocity = MaxSlideVelocity);
	}
	// Set the acceleration of the wall slide
	void SetSlideAcceleration(const float& value) {
		SlideAcceleration = value;
	}
	void SetWallHoldDuration(const float& value) {
		WallHoldDuration = value;
	}

	/****** Getters for private variables ******/
	FORCEINLINE int8 GetWallOnPlayerSide() const {
		return WallOnPlayerSide;
	}
	FORCEINLINE bool GetHangingOnWall() const {
		return HangingOnWall;
	}
	FORCEINLINE bool GetCanDoubleJump() const {
		return CanDoubleJump;
	}
	FORCEINLINE float GetCurrSlideVelocity() const {
		return CurrSlideVelocity;
	}
	FORCEINLINE float GetSlideAcceleration() const {
		return SlideAcceleration;
	}
	FORCEINLINE float GetWallHoldDuration() const {
		return WallHoldDuration;
	}
	
	/****** Turn DoubleJumping on and off ******/
	FORCEINLINE bool IsDoubleJumpEnabled() const {
		return DoubleJumpEnabled;
	}
	void EnableDoubleJumping() {
		DoubleJumpEnabled = true;
		CanDoubleJump = true;
	}
	void DisableDoubleJumping() {
		DoubleJumpEnabled = false;
		CanDoubleJump = false;
	}

/****** Private Information - Jump variables ******/
private:
	// Is the player hanging on the wall
	bool HangingOnWall;
	// Wall direction relative to player
	// -1: Left
	//  1: Right
	//  0: NULL
	int8 WallOnPlayerSide;
	// Can the player double jump; can only be positive if DoubleJumpEnabled is true
	bool CanDoubleJump;
	// Determines if Double Jumping turned on in the game. When false, CanDoubleJump will ALWAYS be false
	bool DoubleJumpEnabled;

	float WallHoldDuration;
};

UCLASS()
class KOA_PROTO_API AKOA_PROTO_Character : public ACharacter
{
	GENERATED_BODY()
/********************* PUBLIC VARIABLES *********************/
public:
	//////////////////////////////////////////////////////////////
	// 						PLAYER STATS 						//
	////////////////////////////////////////////////////////////// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Info")
	FString PlayerName;
    
	//----------------------- STATS ----------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stats")
	float HealthCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stats")
	float HealthMax;
    
	//----------------------- MOVEMENT -------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float RunSpeed;
	UPROPERTY(EditAnywhere, Category = "Player|Jump")
	FPlayerJumpVariables JumpStats;
	//////////////////////////////////////////////////////////////
	// 						Throwables 							//
	//////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Throwables")
	bool IsAimingThrowable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Throwables")
	ETYPEOF_Throwable CurrentThrowable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Throwables")
	TArray<TSubclassOf<UAMTA_BASE_Throwable>> CollectedThrowables;
	
	//////////////////////////////////////////////////////////////
	// 						STATUS EFFECTS 						//
	////////////////////////////////////////////////////////////// 
	//------------------------- BLEED --------------------------//
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|BLEED")
	float SE_BLEED_BuildUp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "StatusEffect|BLEED")
	float SE_BLEED_MaxAmount;
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|BLEED")
	bool IsBLEEDING;
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|BLEED")
	FTimerHandle BLEEDTimerHandle;
	//------------------------- BURN ---------------------------//
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|BURN")
	float SE_BURN_BuildUp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "StatusEffect|BURN")
	float SE_BURN_MaxAmount;
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|BURN")
	bool IsBURNING;
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|BURN")
	FTimerHandle BURNTimerHandle;
	//----------------------- POISON ---------------------------//
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|POISON")
	float SE_POISON_BuildUp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "StatusEffect|POISON")
	float SE_POISON_MaxAmount;
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|POISON")
	bool IsPOISONED;
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect|POISON")
	FTimerHandle POISONTimerHandle;
	//////////////////////////////////////////////////////////////
	// 					ADDITIONAL MESHES 						//
	//////////////////////////////////////////////////////////////
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	UCapsuleComponent* VD_E_AimingCapsule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|E|Mesh")
	USkeletalMeshComponent* VD_E_AimingMeshComponent;

	//////////////////////////////////////////////////////////////
	// 						ARTIFACTS 							//
	//////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Player|Artifacts")
	EArtifactID CurrentArtifact;
	UPROPERTY(EditAnywhere, Category = "Player|Artifacts")
	TArray<TSubclassOf<UKOA_BASE_Artifact>> CollectedArtifacts;

/********************* PUBLIC METHODS *********************/
public:
	//////////////////////////////////////////////////////////////
	// 				CONSTRUCTORS AND INITIALIZERS 				//
	//////////////////////////////////////////////////////////////
	AKOA_PROTO_Character(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;// {
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/****** TICK ******/
	virtual void Tick(float DeltaSeconds) override;
	//////////////////////////////////////////////////////////////
	// 						  UTILITY 							//
	//////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utility|Mouse")
	FVector GetMousePositionInPlayerPlane();
	//////////////////////////////////////////////////////////////
	// 						PLAYER STATS 						//
	//////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	void DamagePlayer(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	void HealPlayer(float Amount);
	
	//********************** MOVEMENT **************************//
	void SetMoveSpeedToRun();
	void SetMoveSpeedToWalk();
	void MoveRight(float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsMovementInputDisabled() const;
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetIsMovementInputDisabled(bool IsDisabled);
	
	//*********************** JUMPING **************************//
	FDetectWallHitInfo DetectWall();
	void PlayerJump();
	void PlayerStopJump();
	virtual void Landed(const FHitResult &Hit) override;
	void StartWallSlide();
	void LoseGripAndFall();
	
	//********************** INVENTORY *************************//
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory")
	void OpenInventory();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory|Throwables")
	void ThrowCurrentThrowable();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory|Throwables")
	void AimCurrentThrowable();
	UFUNCTION(BlueprintCallable, Category = "Inventory|Throwables")
	UAMTA_BASE_Throwable* GetCurrThrowableRefernce() const;
	//////////////////////////////////////////////////////////////
	// 						STATUS EFFECTS 						//
	//////////////////////////////////////////////////////////////
	//************************ BLEED ***************************//
	UFUNCTION(BlueprintCallable, Category = "StatusEffect|BLEED")
	void ApplyBLEEDBuildUp(float Amount);
	void ApplyBLEED();
	//************************* BURN ***************************//
	UFUNCTION(BlueprintCallable, Category = "StatusEffect|BURN")
	void ApplyBURNBuildUp(float Amount);
	void ApplyBURN();
	//************************* POISON ***************************//
	UFUNCTION(BlueprintCallable, Category = "StatusEffect|POISON")
	void ApplyPOISONBuildUp(float Amount);
	void ApplyPOISON();
	
	/****** ARTIFACTS ******/
	void EquipDualDaggers();
	void EquipFireGlove();
	void EquipMatterHammer();
	// void EquipLightningBow();
	// void EquipMatterHammer();

	// CURRENT LIGHT ATTACK //
	void UseCurrBasicAttackLight();
	
	// GETTERS //
	FORCEINLINE bool GetIsArtifactSwapLocked() const;
	UFUNCTION(BlueprintCallable, Category = "Artifact")
	UKOA_BASE_Artifact* GetCurrArtifactReference() const;
	UFUNCTION(BlueprintCallable, Category = "Artifact")
	EArtifactID GetEquippedArtifact() const;
	// SETTERS //
	bool SetCurrentArtifact(EArtifactID Artifact);
	void SetCurrArtifactPlayerReference();

	/****** ABILITIES ******/
	UPROPERTY(EditAnywhere)
	EAbilityID AbilityPressed;

	// PRESS CURRENT ABILITY //
	void PressCurrentAbility(EAbilityID AbilityID);
	void PressCurrentAbilityQ();
	void PressCurrentAbilityW();
	void PressCurrentAbilityE();
	void PressCurrentAbilityR();

	// RELEASE CURRENT ABILITY //
	void ReleaseCurrentAbility(EAbilityID AbilityID);
	void ReleaseCurrentAbilityQ();
	void ReleaseCurrentAbilityW();
	void ReleaseCurrentAbilityE();
	void ReleaseCurrentAbilityR();

	// GETTERS //
	FORCEINLINE bool GetIsAbilityUseLocked() const;
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool GetIsCurrentArtifactAbilityOnCooldown(const EAbilityID &AbilityID) const;
	UFUNCTION(BlueprintCallable, Category = "Ability")
	EAbilityID GetWhichAbilityPressed() const;
	// SETTERS //
	void SetWhichAbilityPressed(const EAbilityID &AbilityID);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetIsAbilityUseLocked(bool Value);
	/****** TIMERS ******/
	void StartAbilityLockTimer();
	void StartArtifactSwapLockTimer(const float &Duration);
	void StartWallHoldTimer(const float &Duration);
	void StartWallSlideTimer(const float &Duration);
	void ClearWallHoldTimer();
	void ClearWallSlideTimer();

	// World
	FORCEINLINE const UWorld* GetWorldPtr() const {
		return WorldPtr;
	}
	
	
/********************* PRIVATE VARIABLES *********************/
private:
	/***** MOVEMENT *****/
	bool CanDodge;
	bool IsSlidingDownWall;
	bool IsMovementInputDisabled;

	/***** ABILTIES *****/
	bool IsAbilityUseLocked;
	bool IsArtifactSwapLocked;
	float ArtifactSwapLockDuration;
	float AbilityLockDuration;
	
	/***** TIMERS *****/
	FTimerHandle AbilityLockTimer;
	FTimerHandle ArtifactSwapLockTimer;
	FTimerHandle WallHoldTimer;
	FTimerHandle WallSlideTimer;

	/***** WORLD *****/
	const UWorld* WorldPtr;

/********************* PRIVATE METHODS *********************/
private:
	void UnlockAbilityUse();
	void UnlockArtifactSwap();
	void DEBUG_EquipCurrentArtifact();
};
