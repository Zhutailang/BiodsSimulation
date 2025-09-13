// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuadTreeLibrary.generated.h"

/**
 * 
 */
class IQuadTreePointInterface;


UCLASS(Blueprintable,BlueprintType)
class AOcTreePoint :public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	int32 id;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ExposeOnSpawn=true))
	float Range;
	UPROPERTY(BlueprintReadOnly)
	TArray<UOcTreeNode*> OcTreeArea;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
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

UCLASS(Blueprintable,BlueprintType)
class UOcTreeNode :public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	int32 max_Level=5;
	UPROPERTY(BlueprintReadWrite)
	int32 Cur_Level;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Range;
	UPROPERTY(BlueprintReadOnly)
	FVector RangeOuter;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 SplitCount=10;
	UPROPERTY(BlueprintReadWrite)
	int32 DirectlyPointCount=0;
	UPROPERTY(BlueprintReadWrite)
	TArray<AOcTreePoint*> Points;
	UPROPERTY(BlueprintReadWrite)
	TArray<UOcTreeNode*> ChildNodes;


	//Function

	UFUNCTION(BlueprintCallable)
	static UOcTreeNode* CreateOcTreeNode(FVector NodeLocation,FVector NodeRange, int32 MaxLevel=5, int32 _SplitCount=10);

	void Init(int32 _CurLevel, FVector _Location, FVector _Range)
	{
		Cur_Level=_CurLevel;
		Location=_Location;
		Range=_Range;
		RangeOuter=Range*1.5;
	}
	UFUNCTION(BlueprintCallable)
	bool Insert(AOcTreePoint* NewPoint);

	UFUNCTION(BlueprintCallable)
	void Clear();

	UFUNCTION(BlueprintCallable)
	TArray<AOcTreePoint*> Retrieve(const FVector PointLocation, const float PointRange);
	
	UFUNCTION(BlueprintCallable)
	bool TryRemoveAt(const FVector PointLocation,const float PointRange);

	UFUNCTION(BlueprintCallable)
	bool TryRemove(AOcTreePoint* Point);
			
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetAllLocation();
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetAllRange();

	UFUNCTION(BlueprintCallable)
	bool SpawnRandOcTreePoint(float PointRange,FVector& NewLocation);

	UFUNCTION(BlueprintCallable)
	bool IsOverlap(const FVector NewLocation,const float NewRange);

	UFUNCTION(BlueprintCallable)
	bool MoveNode(AOcTreePoint* Point,FVector NewLocation);

private:
	void RemoveAt(int32 PointIndex);
	void Remove(AOcTreePoint* Point);
	void Split();
	bool TrySplit();
	bool TryMerge();
	void Merge();
	bool IsInRange(FVector NewLocation, float NewRange);
	bool IsInRangeOuter(FVector NewLocation,float NewRange);
	//int32 GetIndex(FVector PointLocation, float PointRange);
	TArray<int32> GetIndex(FVector PointLocation,float PointRange);
	TArray<int32> GetIndexbyloose(FVector PointLocation, float PointRange);
	TArray<UOcTreeNode*> GetContainNode(AOcTreePoint* Point,FVector NewLocation);

};

UCLASS()
class ALGORITHMTEST_API UQuadTreeLibrary : public UObject
{
	GENERATED_BODY()
	
};
