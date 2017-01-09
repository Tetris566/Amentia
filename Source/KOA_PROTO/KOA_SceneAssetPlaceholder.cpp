// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_SceneAssetPlaceholder.h"


// Sets default values
AKOA_SceneAssetPlaceholder::AKOA_SceneAssetPlaceholder(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PlaceholderRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Placeholder Root"));
	PlaceholderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placeholder Mesh"));
	PlaceholderTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Placeholder Text"));

	//PlaceholderMesh->SetRelativeLocation(FVector(0.0f));

	RootComponent = PlaceholderRoot;
	PlaceholderMesh->AttachTo(RootComponent);
	PlaceholderTextComponent->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxLoader(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (BoxLoader.Succeeded()) {
		PlaceholderMesh->SetStaticMesh(BoxLoader.Object);
		PlaceholderMesh->SetRelativeLocation(FVector(0.0f));
		PlaceholderMesh->SetWorldScale3D(FVector(1.0f));
	}
	//

	DepthInScene = ESceneDepth::SD_DEFAULT;
	CustomDepthEnabled = false;
	CustomDepthValue = 0.0f;
	PlaceholderHeight = 0.0f;
	PlaceholderText = FText::FromString("DEFAULT_VALUE");

	PlaceholderTextComponent->SetText(PlaceholderText);
	PlaceholderMesh->SetMaterial(0, DefaultMaterial);
}

// Called when the game starts or when spawned
void AKOA_SceneAssetPlaceholder::BeginPlay()
{
	Super::BeginPlay();
	FVector location = this->GetActorLocation();
	if (location.Z != PlaceholderHeight) location.Z = PlaceholderHeight;
	this->SetActorLocation(location);
}

// Called every frame
void AKOA_SceneAssetPlaceholder::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

#if WITH_EDITOR
void AKOA_SceneAssetPlaceholder::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	// Get all components
	TArray<UActorComponent*> OwnedComponents;
	GetComponents(OwnedComponents);

	// Get the name of the property that changed
	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_SceneAssetPlaceholder, CustomDepthEnabled)) {		
		if (CustomDepthEnabled == true) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Toggled CustomDepthEnabled: True");
			DepthInScene = ESceneDepth::SD_CUSTOM;
			FVector location = this->GetActorLocation();
			location.X = CustomDepthValue;
			this->SetActorLocation(location);
			PlaceholderMesh->SetMaterial(0, CustomDepthMaterial);
		}
		if (CustomDepthEnabled == false) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Toggled CustomDepthEnabled: False");
			DepthInScene = ESceneDepth::SD_DEFAULT;
			FVector location = this->GetActorLocation();
			location.X = 0.0f;
			this->SetActorLocation(location);
			PlaceholderMesh->SetMaterial(0, DefaultMaterial);
		}
	} else if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_SceneAssetPlaceholder, DepthInScene)) {
		FVector location = this->GetActorLocation();
		switch (DepthInScene) {
		case ESceneDepth::SD_FOREGROUND:
			location.X = -200.0f;
			PlaceholderMesh->SetMaterial(0, ForegroundMaterial);
			break;
		case ESceneDepth::SD_PLAYER_PATH:
			location.X = 0.0f;
			PlaceholderMesh->SetMaterial(0, PlayerPathMaterial);
			break;
		case ESceneDepth::SD_BACKGROUND:
			location.X = 200.0f;
			PlaceholderMesh->SetMaterial(0, BackgroundMaterial);
			break;
		default:
			break;
		}
		this->SetActorLocation(location);
	} else if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_SceneAssetPlaceholder, PlaceholderHeight)) {
		FVector location = this->GetActorLocation();
		location.Z = PlaceholderHeight;
		this->SetActorLocation(location);
	} else if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_SceneAssetPlaceholder, CustomDepthValue)) {
		FVector location = this->GetActorLocation();
		location.X = CustomDepthValue;
		this->SetActorLocation(location);
	} else if (propertyName == GET_MEMBER_NAME_CHECKED(AKOA_SceneAssetPlaceholder, PlaceholderText)) {
		PlaceholderTextComponent->SetText(PlaceholderText);
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif