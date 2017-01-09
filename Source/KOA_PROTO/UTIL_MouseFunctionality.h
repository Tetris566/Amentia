// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*	UTIL_MouseFunctionality: 
		A UTILITY class containing ANY functionality that requires the mouse
		that potentially could be required from multiple source.
 */
class KOA_PROTO_API UTIL_MouseFunctionality {
public:
	static FVector GetMousePosInPlayerPlane(const UWorld* World);
private:
	/* This is a utility class. Therefore, there is no need for,
		nor should ANYONE be allowed to create, an instance of 
		a UTIL_MouseFunctionality. As such, access to the constructors
		is denied; you CAN'T make an instance! HA. */
	UTIL_MouseFunctionality() {}
	~UTIL_MouseFunctionality() {}

};
