// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "KOA_BASE_EnemyMovement.h"


void UKOA_BASE_EnemyMovement::InitializeComponent()
{
	Super::InitializeComponent();
	//~~~~~~~~~~~~~~~~~
	//UE_LOG //comp Init!
}

//Tick Comp
void UKOA_BASE_EnemyMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//UE_LOG //custom comp is ticking!!!

}