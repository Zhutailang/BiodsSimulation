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
	FVector Location;//λ��
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FRotator Direction;//����
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity=FVector(0);//�ٶ�
	UPROPERTY(BlueprintReadWrite)
	FVector Force;//���ٶ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RandSeed;//�������
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AlignmentWeight = 1.f;//������Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SeperationWeight = 1.f;//������Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CohesionWeight = 1.f;//�ۼ���Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float VelocityInterpWeight=0.5f;//�ٶȱ仯��ֵ
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MovePointWeight = 1.f;//Ŀ���ǣ����Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoveAvoidWeight = 4.f;//Ŀ������ʱ���ų���Ȩ��
};

UCLASS()
class ALGORITHMTEST_API ABiodsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABiodsManager();

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Init")
	int32 BiodsSpawnCount=0;//���ɸ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	int32 SpawnRadius=1000;//���ɷ�Χ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	int32 SpawnInterval=100;//���ɵľ�����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	float SpawnVelocityMin = 5;//��С�����ٶ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Init")
	float SpawnVelocityMax = 10;//��������ٶ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float ViewAngle = 90;//range:{0,90}��Ұ�Ƕ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	bool forwardTrend = false;//�Ƿ�ֻ��ע��Ұ�ڵĸ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SeparationRadius = 50;//����뾶
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float AlignmentRadius = 100;//����뾶
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float CohesionRadius = 100;//�ۼ��뾶
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float AvoidDistance=200.f;//Ŀ�����ų�뾶


	FSphere SpawnFSphere;

	UPROPERTY(BlueprintReadWrite, Category = "BiodProperty")
	TArray<FBiod> Biods;//���е�Biods

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "BiodProperty")
	float AlignmentWeight=1.f;//������Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SeperationWeight=1.f;//������Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float CohesionWeight=1.f;//�ۼ���Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float MovePointWeight=1.f;//Ŀ���ǣ����Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float MoveAvoidWeight=4.f;//Ŀ������ʱ���ų���Ȩ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float VelocityInterpWeight =0.5f;//�ٶȱ仯��ֵ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float VelocityInterpWave = 0.2f;//�ٶȱ仯��ֵ�Ĳ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float WavePercent=0.01f;//�����ٷֱ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SpeedMin=0.f;//��С�ٶ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BiodProperty")
	float SpeedMax=100.f;//����ٶ�

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	float MovePointChangeInterval=5.f;//Ŀ���������ʱ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	float MovePointRandRange = 1000.f;//Ŀ������ƫ�Ʒ�Χ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	FVector MoveAreaMax;//Ŀ�����������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TargetPoint")
	FVector MoveAreaMin;//Ŀ�����������
	UPROPERTY(BlueprintReadWrite, Category = "TargetPoint")
	FVector MoveDirect;//Ŀ���

	FRandomStream RandSteam;
	//Component
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UInstancedStaticMeshComponent* InstanceStaticMeshComp;



protected:
	virtual void Init();//��Ⱥ��ʼ��
	virtual void BeginPlay() override;
	virtual bool SpawnRandSphere(FSphere& Sphere);//�������������������
	virtual FVector GetRandomPointInSphere(float Radius);//����������ڹ̶��뾶������������
	virtual void TickBiods(float DeltaTime);//��Ⱥ��Tick
	virtual void RandomVector();//�������λ��Ŀ���
	FTimerHandle Timer;

private:
	virtual bool IsOverlapWithBiods(FSphere NewSphere);//�Ƿ���������ص�
	virtual FVector Seperation(const TArray<FBiod>& NearBiods,FBiod Target);//����
	virtual FVector Alignment(const TArray<FBiod>& NearBiods,FBiod Target);//����
	virtual FVector Cohesion(const TArray<FBiod>& NearBiods,FBiod Target);//�ۼ�
	virtual FVector Disrupt(FBiod Target);//Ŀ������ƫ��
	bool IsInit=false;

public:	

	virtual void Tick(float DeltaTime) override;

};
