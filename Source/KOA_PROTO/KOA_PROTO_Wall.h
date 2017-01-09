// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KOA_PROTO_Wall.generated.h"

UENUM(BlueprintType)
enum class EWallFrictionType : uint8 {
	WT_Rough = 0	UMETA(DisplayName = "Rough"),
	WT_Slippery		UMETA(DisplayName = "Slippery"),
	WT_Smooth		UMETA(DisplayName = "Smooth"),
	WT_Sticky		UMETA(DisplayName = "Sticky"),
	MAX				UMETA(DisplayName = "Invalid")
};

USTRUCT()
struct KOA_PROTO_API FWallInformation {
	GENERATED_USTRUCT_BODY()

	// The initial velocity for when you start sliding down the wall
	float InitSlideVelocity;
	// The acceleration of your slide on this walls surface
	float SlideAcceration;
	// The amount of time you can hold onto the wall before you start to slide
	float WallHoldDuration;

public:
	FWallInformation() {
		InitSlideVelocity	= 1.0f;
		SlideAcceration		= 2.0f;
		WallHoldDuration	= 1.0f;
	}
	FWallInformation(float InitVelocity, float Acceleration, float HoldDuration)
		: InitSlideVelocity(InitVelocity), SlideAcceration(Acceleration), WallHoldDuration(HoldDuration){}

	void operator=(const FWallInformation& value) {
		InitSlideVelocity	= value.InitSlideVelocity;
		SlideAcceration		= value.SlideAcceration;
		WallHoldDuration	= value.WallHoldDuration;
	}
};

UCLASS()
class KOA_PROTO_API AKOA_PROTO_Wall : public AActor
{
	GENERATED_BODY()

public:
	// Mesh that contains the actual visible wall
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Wall")
	class UStaticMeshComponent* WallMesh;

	// Capsule to be placed in the scene for the location the player should fall off the wall.
	UPROPERTY(EditInstanceOnly, BlueprintReadonly, Category = "Fall Off Point")
	class UCapsuleComponent* FallOffPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	EWallFrictionType WallType;

public:
	AKOA_PROTO_Wall();
	virtual void BeginPlay() override;

	// Returns the wall info based on this WallType
	FWallInformation GetWallInfo();

	static const FWallInformation RoughWallInfo;
	static const FWallInformation SlipperyWallInfo;
	static const FWallInformation SmoothWallInfo;
	static const FWallInformation StickyWallInfo;
	
	UFUNCTION(Category = "Proximity")
	void FallOffDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Primarily a debug function, used to print the name of the Enum established at the top of this .h
	static FORCEINLINE FString GetEnumValueToString(const FString& EnumName, EWallFrictionType EnumValue) {
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!enumPtr) {
			return FString("Invalid");
		}
		return enumPtr->GetEnumName((int32)EnumValue);
	}
};