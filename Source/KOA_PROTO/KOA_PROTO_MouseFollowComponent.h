// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "KOA_PROTO_MouseFollowComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//UCLASS()
class KOA_PROTO_API UKOA_PROTO_MouseFollowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
	float zeroXPosition;
	UPROPERTY(EditAnywhere)
	float cameraFOV;
	FVector cameraLocation;
	FRotator cameraRotation;
	APlayerController* playerController;

	// Sets default values for this component's properties
	UKOA_PROTO_MouseFollowComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
