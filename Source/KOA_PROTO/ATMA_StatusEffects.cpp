// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "ATMA_StatusEffects.h"

UATMA_StatusEffects::UATMA_StatusEffects() {
	UIBleedIcon = nullptr;
	UIBurnIcon = nullptr;
	UIPoisonIcon = nullptr;
}	

UATMA_StatusEffects::~UATMA_StatusEffects() {}

/*
#if WITH_EDITOR
voidUATMA_StatusEffects::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	// Get the name of the property that changed
	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(UATMA_StatusEffects, HasBleed) {
		if (HasBleed == true) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Toggled HasBleed: True");
		}
	}
	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
*/