// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ATMA_StatusEffects.h"
#include "BASE_Projectile.generated.h"

UENUM(BlueprintType)
enum class EProjectileTrajectory : uint8 {
	PT_LINEAR = 0 	UMETA(DisplayName="Linear"),
	PT_PARABOLIC	UMETA(DisplayName="Parabola"),
	NUM				UMETA(Hidden),
	NONE			UMETA(DisplayName="None")
};

UCLASS()
class KOA_PROTO_API ABASE_Projectile : public AActor
{
	GENERATED_BODY()
public:
	// STATS //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjMaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EProjectileTrajectory ProjTrajectory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|StatusEffects")
	FStatusEffects ProjStatusEffects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* ProjMesh;
public:	
	// Sets default values for this actor's properties
	ABASE_Projectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private: 
	float Gravity;
};
