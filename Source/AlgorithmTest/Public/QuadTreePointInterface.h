// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuadTreePointInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UQuadTreePointInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALGORITHMTEST_API IQuadTreePointInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Activate();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Deactivate();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetLocation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetLocation(FVector Location);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetRange();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeletePoint();

};
