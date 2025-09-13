// Fill out your copyright notice in the Description page of Project Settings.


#include "BiodsLibrary.h"
#include "BiodsManager.h"

FVector UBiodsLibrary::Seperation(TArray<FBiod> Biods, FBiod target)
{
	FVector targetDirection=FVector(0);
	for (auto it : Biods)
	{
		targetDirection+=it.Location;
	}
	return targetDirection;
}

FRotator UBiodsLibrary::Alignment(TArray<FBiod> Biods, FBiod target)
{
	FQuat CombineQuat=FQuat::Identity;
	for (auto it : Biods)
	{
		CombineQuat+=it.Direction.Quaternion();
	}
	CombineQuat/=Biods.Num();
	CombineQuat.Normalize();
	return CombineQuat.Rotator();
}

FVector UBiodsLibrary::Cohesion(TArray<FBiod> Biods, FBiod target)
{
	FVector targetDirection=FVector(0);
	for (auto it : Biods)
	{
		targetDirection+=it.Location;
	}
	return targetDirection/Biods.Num();
}

bool UBiodsLibrary::GetLeisureLocation(TArray<FBiod> Biods,FBiod targetBiod, FVector SpawnLoc, float SpawnRadius,float DistanceFiled)
{
	return true;
}
