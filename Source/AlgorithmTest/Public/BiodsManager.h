// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BiodsManager.generated.h"

class UArrowComponent;
class UInsightsSkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FBiod
{
	GENERATED_BODY()
public:
	FBiod(){}

	FBiod(int32 _id,
	FVector _Location,
	FRotator _Direction)
	{
		id=_id;
		Location=_Location;
		Direction=_Direction;
		RandSeed=(int32)((Location.X+Location.Y+Location.Z)*1000000)%1000000;
	}

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 id;//ID
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector Location;//位置
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FRotator Direction;//朝向
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity=FVector(0);//速度
	UPROPERTY(BlueprintReadWrite)
	FVector Force;//加速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RandSeed;//随机种子
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AlignmentWeight = 1.f;//对齐力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SeperationWeight = 1.f;//分离力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CohesionWeight = 1.f;//聚集力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float VelocityInterpWeight=0.5f;//速度变化插值
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MovePointWeight = 1.f;//目标点牵引力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoveAvoidWeight = 4.f;//目标点过近时的排斥力权重
};

UCLASS()
class ALGORITHMTEST_API ABiodsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABiodsManager();

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Init")
	int32 BiodsSpawnCount=0;//生成个数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	int32 SpawnRadius=1000;//生成范围
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	int32 SpawnInterval=100;//生成的距离间隔
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	float SpawnVelocityMin = 5;//最小生成速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	float SpawnVelocityMax = 10;//最大生成速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float ViewAngle = 90;//range:{0,90}视野角度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	bool forwardTrend = false;//是否只关注视野内的个体
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SeparationRadius = 50;//分离半径
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float AlignmentRadius = 100;//对齐半径
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float CohesionRadius = 100;//聚集半径
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float AvoidDistance=200.f;//目标点的排斥半径


	FSphere SpawnFSphere;

	UPROPERTY(BlueprintReadWrite, Category = "BiodProperty")
	TArray<FBiod> Biods;//所有的Biods

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "BiodProperty")
	float AlignmentWeight=1.f;//对齐力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SeperationWeight=1.f;//分离力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float CohesionWeight=1.f;//聚集力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float MovePointWeight=1.f;//目标点牵引力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float MoveAvoidWeight=4.f;//目标点过近时的排斥力权重
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float VelocityInterpWeight =0.5f;//速度变化插值
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float VelocityInterpWave = 0.2f;//速度变化插值的波动
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float WavePercent=0.01f;//波动百分比
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SpeedMin=0.f;//最小速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SpeedMax=100.f;//最大速度

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	float MovePointChangeInterval=5.f;//目标点更换间隔时间
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	float MovePointRandRange = 1000.f;//目标点随机偏移范围
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	FVector MoveAreaMax;//目标点生成区域
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	FVector MoveAreaMin;//目标点生成区域
	UPROPERTY(BlueprintReadWrite, Category = "TargetPoint")
	FVector MoveDirect;//目标点

	FRandomStream RandSteam;
	//Component
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UInstancedStaticMeshComponent* InstanceStaticMeshComp;



protected:
	virtual void Init();//鸟群初始化
	virtual void BeginPlay() override;
	virtual bool SpawnRandSphere(FSphere& Sphere);//在生成区域生成随机球
	virtual FVector GetRandomPointInSphere(float Radius);//生成随机点在固定半径的球形区域内
	virtual void TickBiods(float DeltaTime);//鸟群的Tick
	virtual void RandomVector();//生成随机位置目标点
	FTimerHandle Timer;

private:
	virtual bool IsOverlapWithBiods(FSphere NewSphere);//是否和其他鸟重叠
	virtual FVector Seperation(const TArray<FBiod>& NearBiods,FBiod Target);//分离
	virtual FVector Alignment(const TArray<FBiod>& NearBiods,FBiod Target);//对齐
	virtual FVector Cohesion(const TArray<FBiod>& NearBiods,FBiod Target);//聚集
	virtual FVector Disrupt(FBiod Target);//目标点随机偏移
	bool IsInit=false;

public:	

	virtual void Tick(float DeltaTime) override;

};
