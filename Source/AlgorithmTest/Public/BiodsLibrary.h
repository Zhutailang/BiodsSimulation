// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BiodsLibrary.generated.h"

/**
 * 
 */

struct FBiod;

UCLASS()
class ALGORITHMTEST_API UBiodsLibrary : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static FVector Seperation(TArray<FBiod> Biods,FBiod target);

	UFUNCTION(BlueprintCallable)
	static FRotator Alignment(TArray<FBiod> Biods, FBiod target);

	UFUNCTION(BlueprintCallable)
	static FVector Cohesion(TArray<FBiod> Biods, FBiod target);

	UFUNCTION(BlueprintCallable)
	static bool GetLeisureLocation(TArray<FBiod> Biods,FBiod targetBiod, FVector SpawnLoc,float SpawnRadius = 200.f, float DistanceFiled = 20.f);
};
