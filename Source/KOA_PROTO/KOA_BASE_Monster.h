// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "KOA_BASE_Monster.generated.h"

UCLASS()
class KOA_PROTO_API AKOA_BASE_Monster : public APawn {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterMesh")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterProperties")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterProperties")
	float HitPoints;

	// CapsuleComponent for collision
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	UCapsuleComponent* CollisionCapsule;

	// Monster sight range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	float SightRange;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* SightSphere;

	// Monster attack range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	float AttackRange;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* AttackRangeSphere;

public:
	// Sets default values for this pawn's properties
	AKOA_BASE_Monster(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Override to update changes made in the editor
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
