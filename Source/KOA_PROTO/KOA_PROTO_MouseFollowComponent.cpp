// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_PROTO_MouseFollowComponent.h"


// Sets default values for this component's properties
UKOA_PROTO_MouseFollowComponent::UKOA_PROTO_MouseFollowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKOA_PROTO_MouseFollowComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKOA_PROTO_MouseFollowComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	playerController = GetWorld()->GetFirstPlayerController();
	playerController->bShowMouseCursor = true;
	cameraFOV = playerController->PlayerCameraManager->GetFOVAngle();
	cameraLocation = playerController->PlayerCameraManager->GetCameraLocation();
	cameraRotation = playerController->PlayerCameraManager->GetCameraRotation();

	FIntPoint nCursorPos;
	FViewport* viewport = CastChecked<ULocalPlayer>(playerController->Player)->ViewportClient->Viewport;
	viewport->GetMousePos(nCursorPos);
	FVector2D fCursorPos;
	float screenWidth = (float)viewport->GetSizeXY().X;
	float screenHeight = (float)viewport->GetSizeXY().Y;
	FTransform cameraTransform = FTransform(cameraRotation.Quaternion(), cameraLocation, FVector(1, 1, 1));
	
	//Calculate the cursor position as a 2D float vector. The center of the screen will be 0, 0, the left side will be -0.5 and the right side will be 0.5. 0.5 vertically will be half of the screen width up from the center.
	fCursorPos.X = nCursorPos.X / screenWidth - 0.5f;
	fCursorPos.Y = -(nCursorPos.Y / screenHeight - 0.5f) * screenHeight / screenWidth;

	//Do some trig to calculate the correct vector of the ray to cast
	FVector projectedVector = FVector(
		0.5f / FMath::Tan(FMath::DegreesToRadians(cameraFOV / 2)), 
		fCursorPos.X, 
		fCursorPos.Y 
	);

	//Transform the projected vector into the camera's local space
	projectedVector = cameraTransform.TransformVector(projectedVector);

	//Calculate final position
	FVector finalPos;
	finalPos.X = zeroXPosition;
	finalPos.Y = (zeroXPosition - cameraLocation.X) / projectedVector.X * projectedVector.Y + cameraLocation.Y;
	finalPos.Z = (zeroXPosition - cameraLocation.X) / projectedVector.X * projectedVector.Z + cameraLocation.Z;

	this->GetOwner()->SetActorLocation(finalPos);
}

