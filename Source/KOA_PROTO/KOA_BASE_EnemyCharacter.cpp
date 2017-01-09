// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_BASE_EnemyCharacter.h"
#include "KOA_BASE_EnemyMovement.h"


// Sets default values
AKOA_BASE_EnemyCharacter::AKOA_BASE_EnemyCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UKOA_BASE_EnemyMovement>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Init variables to default value //
	// STATS //
	Name = "INVALID_ENEMY_NAME";
	EnemyID = ETypeOfEnemy::NONE;
	
	// -- HP -- //
	HPCurr = 75.0f;
	HPMax = 100.0f;
	// STATUS EFFECTS //
	/* BLEED */
	SE_BleedBuildUp = 0.0f;
	SE_BleedMaxAmount = 100.0f;
	IsBleeding = false;
	// -- STATUS -- //
	IsDead = false;
	// -- MOVEMENT -- //
	Speed = 0.0f;

}

// Called when the game starts or when spawned
void AKOA_BASE_EnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	
}
void AKOA_BASE_EnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	SE_BleedBuildUp = 0.0;
	IsBleeding = false;
	IsDead = false;
}
// Called every frame
void AKOA_BASE_EnemyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// If HP ever drops below 0.0, you die 
	if (HPCurr <= 0.0 && !IsDead) {
		OnDeath();
	}
}

// Called to bind functionality to input
void AKOA_BASE_EnemyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AKOA_BASE_EnemyCharacter::ReceiveDamage(float Amount) {
	HPCurr -= Amount;
}
void AKOA_BASE_EnemyCharacter::ApplyBleedBuildUp(float Amount) {
	if (SE_BleedBuildUp + Amount >= SE_BleedMaxAmount) {
		SE_BleedBuildUp = 0.0;
		IsBleeding = true;
		
	} else {
		SE_BleedBuildUp += Amount;
	}
}
void AKOA_BASE_EnemyCharacter::ApplyBleed() {
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.7f, FColor::Red, "ApplyBleed");
}

void AKOA_BASE_EnemyCharacter::OnDeath() {
	IsDead = true;
}

//#if WITH_EDITOR
//void AKOA_BASE_EnemyCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
//	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//	if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_BASE_EnemyCharacter, SightRange)) {
//		SightSphere->SetSphereRadius(SightRange);
//	}
//	else if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_BASE_EnemyCharacter, AttackRange)) {
//		AttackRangeSphere->SetSphereRadius(AttackRange);
//	}
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//}
//#endif