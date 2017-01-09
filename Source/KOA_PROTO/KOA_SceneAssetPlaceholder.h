// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KOA_SceneAssetPlaceholder.generated.h"

UENUM(BlueprintType)
enum class ESceneDepth : uint8 {
	SD_DEFAULT = 0 UMETA(Hidden),
	SD_FOREGROUND,
	SD_PLAYER_PATH,
	SD_BACKGROUND,
	SD_CUSTOM = 99 UMETA(Hidden)
};

UCLASS()
class KOA_PROTO_API AKOA_SceneAssetPlaceholder : public AActor
{
	GENERATED_BODY()
public:

	USceneComponent* PlaceholderRoot;

	UPROPERTY(EditAnywhere, Category = "Placeholder,Mesh")
	UStaticMeshComponent* PlaceholderMesh;

	UPROPERTY(EditAnywhere, Category = "Placeholder,Text")
	FText PlaceholderText;

	UPROPERTY(EditAnywhere, Category = "Placeholder,Text")
	UTextRenderComponent* PlaceholderTextComponent;

	UPROPERTY(EditAnywhere, Category = "Placeholder,Material")
	UMaterialInterface* DefaultMaterial;
	UPROPERTY(EditAnywhere, Category = "Placeholder,Material")
	UMaterialInterface* ForegroundMaterial;
	UPROPERTY(EditAnywhere, Category = "Placeholder,Material")
	UMaterialInterface* PlayerPathMaterial;
	UPROPERTY(EditAnywhere, Category = "Placeholder,Material")
	UMaterialInterface* BackgroundMaterial;
	UPROPERTY(EditAnywhere, Category = "Placeholder,Material")
	UMaterialInterface* CustomDepthMaterial;

	UPROPERTY(EditAnywhere, Category = "Placeholder, Height")
	float PlaceholderHeight;

	UPROPERTY(EditAnywhere, Category = "Placeholder,Scene Depth", meta = (EditCondition = "!CustomDepthEnabled"))
	ESceneDepth DepthInScene;

	UPROPERTY(EditAnywhere, Category = "Placeholder,Scene Depth")
	bool CustomDepthEnabled;

	UPROPERTY(EditAnywhere, Category = "Placeholder,Scene Depth", meta=(EditCondition="CustomDepthEnabled")) 
	float CustomDepthValue;

public:	
	// Sets default values for this actor's properties
	AKOA_SceneAssetPlaceholder(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
