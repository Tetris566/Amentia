// Fill out your copyright notice in the Description page of Project Settings.

#include "KOA_PROTO.h"
#include "AMTA_BASE_Throwable.h"

UAMTA_BASE_Throwable::UAMTA_BASE_Throwable() {
	TypeOf = ETYPEOF_Throwable::NONE;
	Icon = nullptr;
	Mesh = nullptr;

	Damage = 0.0f;
}
UAMTA_BASE_Throwable::~UAMTA_BASE_Throwable() {

}


