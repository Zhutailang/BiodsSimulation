// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridAlgorithmLibrary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class AGridActor :public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ExposeOnSpawn=true))
	FBox2D Size;
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector2D> OwnGrid;
	UPROPERTY(BlueprintReadOnly)
	USpaceGrid* Manager;
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Activate();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Deactivate();
};

/*
* 通用父类
*/
UCLASS(Abstract, Blueprintable, BlueprintType)
class USpaceGrid :public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	FVector2D AreaMin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	FVector2D AreaMax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	int32 GridSize = 10000;
	UPROPERTY(BlueprintReadOnly)
	int32 GridXcount;
	UPROPERTY(BlueprintReadOnly)
	int32 GridYcount;


	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static USpaceGrid* CreateSpaceGrid(UObject* WorldContextObject,TSubclassOf<USpaceGrid> Class,FVector2D _AreaMin, FVector2D _AreaMax, int32 _GridSize = 10000);
	UFUNCTION(BlueprintCallable)
	TArray<AGridActor*> Retrieve(FBox2D Box);
	virtual TArray<AGridActor*> Retrieve_Implementation(FBox2D Box);
	UFUNCTION(BlueprintCallable)
	bool Insert(AGridActor* GridActor);
	virtual bool Insert_Implementation(AGridActor* GridActor); 
	UFUNCTION(BlueprintCallable)
	bool Remove(AGridActor* GridActor);
	virtual bool Remove_Implementation(AGridActor* GridActor);
	UFUNCTION(BlueprintCallable)
	bool MoveActor(AGridActor* GridActor, FVector2D NewLocation);
	virtual bool MoveActor_Implementation(AGridActor* GridActor, FVector2D NewLocation);
	UFUNCTION(BlueprintCallable)
	bool SpawnRandomPoint(float BoxSize, FBox2D& NewBox);
	virtual bool SpawnRandomPoint_Implementation(float BoxSize, FBox2D& NewBox);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsOverlap(FBox2D Size);
	virtual bool IsOverlap_Implementation(FBox2D Size);
protected:
	virtual void Init();
	virtual void Clear();

};

/*
* 均与网格管理系统
*/
UCLASS(Blueprintable)
class UGridDataManager :public USpaceGrid
{
	GENERATED_BODY()
public:

	TArray<TArray<TArray<AGridActor*>>> DataArray;

	virtual TArray<AGridActor*> Retrieve_Implementation(FBox2D Box);

	virtual bool Insert_Implementation(AGridActor* GridActor);

	virtual bool Remove_Implementation(AGridActor* GridActor);

	virtual bool MoveActor_Implementation(AGridActor* GridActor,FVector2D NewLocation);

	virtual bool SpawnRandomPoint_Implementation(float BoxSize,FBox2D& NewBox);

	virtual bool IsOverlap_Implementation(FBox2D Size);
private:
	virtual void Init();
	
};

/*
* 稀疏网格管理系统
*/
UCLASS(BlueprintType)
class UHashGridDataManger :public USpaceGrid
{
	GENERATED_BODY()
public:

	int32 Table_size=4096;
	TArray<TArray<AGridActor*>> HashBuckets;


	virtual TArray<AGridActor*> Retrieve_Implementation(FBox2D Box);

	virtual bool Insert_Implementation(AGridActor* GridActor);

	virtual bool Remove_Implementation(AGridActor* GridActor);

	virtual bool MoveActor_Implementation(AGridActor* GridActor, FVector2D NewLocation);

	virtual bool SpawnRandomPoint_Implementation(float BoxSize, FBox2D& NewBox);

	virtual bool IsOverlap_Implementation(FBox2D Size);


private:
	virtual void Init();
	virtual void Clear();

	int32 calculateHashKey(int32 x,int32 y,int32 z);
};

UCLASS()
class ALGORITHMTEST_API UGridAlgorithmLibrary : public UObject
{
	GENERATED_BODY()
	
};
