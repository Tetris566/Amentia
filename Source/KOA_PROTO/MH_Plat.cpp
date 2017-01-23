// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "MH_Plat.h"


// Sets default values
AMH_Plat::AMH_Plat(const class FObjectInitializer &PCIP) : Super(PCIP)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add mesh component
	PlatformMesh = PCIP.CreateDefaultSubobject<UStaticMesh>(this, TEXT("PlatformMesh"));

	//PlatformMesh->bHiddenInGame = false;
	//RootComponent = PlatformMesh;

}

void AMH_Plat::DestroyMe() {
	Destroy();
}

// Called when the game starts or when spawned
void AMH_Plat::BeginPlay()
{
	Super::BeginPlay();

	//Destroy this after 3 seconds.
	GetWorld()->GetTimerManager().SetTimer(PlatDestruct, this, &AMH_Plat::DestroyMe, 3.0f, false);
}

// Called every frame
void AMH_Plat::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

