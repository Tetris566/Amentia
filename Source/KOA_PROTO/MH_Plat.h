// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MH_Plat.generated.h"

UCLASS()
class KOA_PROTO_API AMH_Plat : public AActor
{
	GENERATED_UCLASS_BODY()

public:	

	UPROPERTY(EditAnywhere,  Category = "Mesh")
	UStaticMesh* PlatformMesh;

	FTimerHandle PlatDestruct;

	// Sets default values for this actor's properties
	AMH_Plat();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void DestroyMe();
	
};
