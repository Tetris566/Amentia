// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "UTIL_MouseFunctionality.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Get mouse cursor position in relation to the player's plane.
FVector UTIL_MouseFunctionality::GetMousePosInPlayerPlane(const UWorld* World) {
	// Get Camera variables
	APlayerController* playerController = World->GetFirstPlayerController();
	float	 cameraFOV = playerController->PlayerCameraManager->GetFOVAngle();
	FVector	 cameraLocation = playerController->PlayerCameraManager->GetCameraLocation();
	FRotator cameraRotation = playerController->PlayerCameraManager->GetCameraRotation();

	// Get the viewport
	FIntPoint nCursorPos;
	FViewport* viewport = CastChecked<ULocalPlayer>(playerController->Player)->ViewportClient->Viewport;

	// Get Mouse Position in Int
	viewport->GetMousePos(nCursorPos);
	// Get the screen dimensions
	FIntPoint screenDimension = viewport->GetSizeXY();
	// Get the camera transform.
	FTransform cameraTransform = FTransform(cameraRotation.Quaternion(), cameraLocation, FVector(1, 1, 1));

	// Calculate the curor position as a 2D float vector. 
	// The center of the screen will be (0,0)
	// The left side will be -0.5 and the right side will be 0.5
	// vertically will be half of the screen width up from the center.
	FVector2D fCursorPos;
	fCursorPos.X = (nCursorPos.X / (float)screenDimension.X) - 0.5f;
	fCursorPos.Y = -((nCursorPos.Y / (float)screenDimension.Y) - 0.5f) * ((float)screenDimension.Y / screenDimension.X);

	// Do some trig to calculate the correct vector of the ray to cast
	FVector projectedVector = FVector(
		0.5f / FMath::Tan(FMath::DegreesToRadians(cameraFOV / 2)),
		fCursorPos.X,
		fCursorPos.Y
	);

	// Transform the projected vector into the camera's local space
	projectedVector = cameraTransform.TransformVector(projectedVector);

	// Calculate and return final position
	FVector finalPos;
	finalPos.X = 0.0f;
	finalPos.Y = ((0.0f - cameraLocation.X) / projectedVector.X) * projectedVector.Y + cameraLocation.Y;
	finalPos.Z = ((0.0f - cameraLocation.X) / projectedVector.X) * projectedVector.Z + cameraLocation.Z;
	return finalPos;
}
