// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_BASE_Monster.h"


// Sets default values
AKOA_BASE_Monster::AKOA_BASE_Monster(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init variables to default value
	Speed = 1.0f;
	HitPoints = 3.0f;
	SightRange = 1200.0f;
	AttackRange = 175.0f;

	// Create the CollisionCapsule and set as root
	CollisionCapsule = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CollisionCapsule"));
	RootComponent = CollisionCapsule;

	// Create the Monster Mesh
	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(CollisionCapsule);

	// Create the monster's SightSphere and attach it to the root
	SightSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SightSphere"));
	SightSphere->SetSphereRadius(SightRange);
	SightSphere->AttachTo(CollisionCapsule);

	// Create the monster's AttackRangeSphere
	AttackRangeSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("AttackRangeSphere"));
	AttackRangeSphere->AttachTo(CollisionCapsule);
	AttackRangeSphere->SetSphereRadius(AttackRange);
}

// Called when the game starts or when spawned
void AKOA_BASE_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKOA_BASE_Monster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AKOA_BASE_Monster::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

#if WITH_EDITOR
void AKOA_BASE_Monster::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_BASE_Monster, SightRange)) {
		SightSphere->SetSphereRadius(SightRange);
	}
	else if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_BASE_Monster, AttackRange)) {
		AttackRangeSphere->SetSphereRadius(AttackRange);
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif