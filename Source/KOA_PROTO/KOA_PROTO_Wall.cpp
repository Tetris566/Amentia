// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_PROTO_Wall.h"

// FWallInformation( InitSlideVelocity, SlideAcceration, WallHoldDuration)
const FWallInformation AKOA_PROTO_Wall::RoughWallInfo		= FWallInformation(1.0f, 75.0f, 1.5f);
const FWallInformation AKOA_PROTO_Wall::SlipperyWallInfo	= FWallInformation(100.0f, 1300.0f, 0.05f);
const FWallInformation AKOA_PROTO_Wall::SmoothWallInfo		= FWallInformation(3.0f, 198.0f, 0.85f);
const FWallInformation AKOA_PROTO_Wall::StickyWallInfo		= FWallInformation(1.0f, 25.0f, 2.0f);

// Sets default values
AKOA_PROTO_Wall::AKOA_PROTO_Wall() {
 	PrimaryActorTick.bCanEverTick = false;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	RootComponent = WallMesh;

	FallOffPoint = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Fall Off Point"));
	FallOffPoint->AttachToComponent(WallMesh, FAttachmentTransformRules::KeepWorldTransform);
	// Add our FallOffDetection function to run when an actor begins to overlap it.
	FallOffPoint->OnComponentBeginOverlap.AddDynamic(this, &AKOA_PROTO_Wall::FallOffDetection);
}
// Called when the game starts or when spawned
void AKOA_PROTO_Wall::BeginPlay() {
	Super::BeginPlay();
}

FWallInformation AKOA_PROTO_Wall::GetWallInfo() {
	FWallInformation wallInfo;
	switch (this->WallType) {
		case EWallFrictionType::WT_Rough:
			wallInfo = RoughWallInfo;
			break;
		case EWallFrictionType::WT_Slippery:
			wallInfo = SlipperyWallInfo;
			break;
		case EWallFrictionType::WT_Smooth:
			wallInfo = SmoothWallInfo;
			break;
		case EWallFrictionType::WT_Sticky:
			wallInfo = StickyWallInfo;
			break;
		default:
			wallInfo = RoughWallInfo;
			break;
	}
	return wallInfo;
}

void AKOA_PROTO_Wall::FallOffDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SOMETHING HIT ME COLLIDER");
}
