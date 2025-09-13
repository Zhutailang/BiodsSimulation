// Fill out your copyright notice in the Description page of Project Settings.


#include "BiodsManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TimerManager.h"


ABiodsManager::ABiodsManager()
{

	PrimaryActorTick.bCanEverTick = true;
	InstanceStaticMeshComp=CreateDefaultSubobject<UInstancedStaticMeshComponent>(L"InstanceStaticMesh");
}

void ABiodsManager::Init()
{

	//球体生成区域
	SpawnFSphere.Center=GetActorLocation();
	SpawnFSphere.W=SpawnRadius;
	FSphere Temp;

	//Biods初始化
	for (int32 i = 0; i < BiodsSpawnCount; i++)
	{
		if (SpawnRandSphere(Temp))//在生成区域内生成随机位置的球体积
		{
			FBiod Biod(i,Temp.Center,GetActorRotation());
			RandSteam.Initialize(Biod.RandSeed);//随机流使用当前Biod的种子

			//每个Biod的个体差异
			Biod.VelocityInterpWeight =VelocityInterpWeight+RandSteam.FRandRange(-VelocityInterpWave,VelocityInterpWave);
			Biod.AlignmentWeight=AlignmentWeight+RandSteam.FRandRange(-WavePercent*AlignmentWeight,WavePercent*AlignmentWeight);
			Biod.CohesionWeight=CohesionWeight+RandSteam.FRandRange(-WavePercent*CohesionWeight,WavePercent*CohesionWeight);
			Biod.SeperationWeight=SeperationWeight+RandSteam.FRandRange(-WavePercent*SeperationWeight,WavePercent*SeperationWeight);
			Biod.MoveAvoidWeight=MoveAvoidWeight+RandSteam.FRandRange(-WavePercent*MoveAvoidWeight,WavePercent*MoveAvoidWeight);
			Biod.MovePointWeight=MovePointWeight+RandSteam.FRandRange(-WavePercent*MovePointWeight,WavePercent*MovePointWeight);
			//每个Biod的个体差异

			//随机生成速度
			FVector TempVelocity;
			TempVelocity.X= RandSteam.FRandRange(-1.f, 1.f);
			TempVelocity.Y= RandSteam.FRandRange(-1.f,1.f);
			TempVelocity.Z= RandSteam.FRandRange(-1.f,1.f);
			TempVelocity.Normalize();
			TempVelocity*= RandSteam.RandRange(SpawnVelocityMin,SpawnVelocityMax);

			//静态网格体实例初始化
			Biods.Add(Biod);
			FTransform TempTrans;
			TempTrans.SetLocation(Biod.Location);
			TempTrans.SetRotation(TempVelocity.Rotation().Quaternion());
			TempTrans.SetScale3D(FVector(0.1));
			InstanceStaticMeshComp->AddInstance(TempTrans,true);
		}
		else break;
	}

	//随机目标点
	RandomVector();
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ABiodsManager::RandomVector, MovePointChangeInterval, true);
	IsInit=true;
}

void ABiodsManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

bool ABiodsManager::SpawnRandSphere(FSphere& Sphere)
{
	Sphere.Center=GetActorLocation()+GetRandomPointInSphere(SpawnRadius);
	Sphere.W=SpawnInterval;
	int32 Count=0;
	while (IsOverlapWithBiods(Sphere))
	{
		if(Count>200)
			return false;
		Sphere.Center= GetActorLocation() + GetRandomPointInSphere(SpawnRadius);
		Sphere.W= SpawnInterval;
		Count++;
	}
	return true;
}

FVector ABiodsManager::GetRandomPointInSphere(float Radius)
{
	// 生成单位球内的随机点（均匀分布）
	float Theta = FMath::FRand() * 2 * PI;
	float Phi = FMath::Acos(2 * FMath::FRand() - 1);
	float R = FMath::Pow(FMath::FRand(), 1.0f / 3.0f) * Radius; // 立方根缩放保证均匀

	float SinPhi = FMath::Sin(Phi);

	return FVector(
		R * SinPhi * FMath::Cos(Theta),
		R * SinPhi * FMath::Sin(Theta),
		R * FMath::Cos(Phi)
	);
}

void ABiodsManager::TickBiods(float DeltaTime)
{
	if(!IsInit)	return;

	for (int32 i = 0; i < Biods.Num(); i++)
	{
		//Calculate
		FSphere Temp;
		Temp.Center=Biods[i].Location;
		TArray<FBiod> AlignmmetBiods;
		TArray<FBiod> CohesionBiods;
		TArray<FBiod> SeperationBiods;
		//寻找附近的Biods
		for (int32 j = 0; j < Biods.Num(); j++)
		{
			if(i==j) continue;
			Temp.W = AlignmentRadius;
			if(Temp.IsInside(Biods[j].Location))
				AlignmmetBiods.Add(Biods[j]);
			Temp.W=CohesionRadius;
			if(Temp.IsInside(Biods[j].Location))
				CohesionBiods.Add(Biods[j]);
			Temp.W=SeparationRadius;
			if(Temp.IsInside(Biods[j].Location))
				SeperationBiods.Add(Biods[j]);
		}

		//计算加速度
		Biods[i].Force = Seperation(SeperationBiods, Biods[i]) * Biods[i].SeperationWeight
			+ Alignment(AlignmmetBiods, Biods[i]) * Biods[i].AlignmentWeight
			+ Cohesion(CohesionBiods, Biods[i]) * Biods[i].CohesionWeight;
		Biods[i].Force +=(MoveDirect+ Disrupt(Biods[i]) -Biods[i].Location).Length()>AvoidDistance?(MoveDirect + Disrupt(Biods[i]) -Biods[i].Location)* Biods[i].MovePointWeight:(Biods[i].Location-MoveDirect + Disrupt(Biods[i]))* Biods[i].MoveAvoidWeight;
		
		//Update
		Biods[i].Velocity= FMath::VInterpTo(Biods[i].Velocity, (Biods[i].Velocity + Biods[i].Force),DeltaTime, Biods[i].VelocityInterpWeight);
		Biods[i].Velocity=Biods[i].Velocity.GetSafeNormal()*FMath::Clamp(Biods[i].Velocity.Length(),SpeedMin,SpeedMax);
		Biods[i].Location+=Biods[i].Velocity*DeltaTime;
		Biods[i].Direction=Biods[i].Velocity.Rotation();
		FTransform TempTrans;
		TempTrans.SetLocation(Biods[i].Location);
		TempTrans.SetRotation(Biods[i].Direction.Quaternion());
		TempTrans.SetScale3D(FVector(0.1));
		InstanceStaticMeshComp->UpdateInstanceTransform(i,TempTrans,true);
	}
}

void ABiodsManager::RandomVector()
{
	FVector Temp;
	Temp.X=FMath::RandRange(MoveAreaMin.X,MoveAreaMax.X);
	Temp.Y=FMath::RandRange(MoveAreaMin.Y,MoveAreaMax.Y);
	Temp.Z=FMath::RandRange(MoveAreaMin.Z,MoveAreaMax.Z);
	MoveDirect=Temp;
}

bool ABiodsManager::IsOverlapWithBiods(FSphere NewSphere)
{
	FSphere Temp;
	for (int32 i = 0; i < Biods.Num(); i++)
	{
		Temp.Center=Biods[i].Location;
		Temp.W= SpawnInterval;
		if(NewSphere.Intersects(Temp))
			return true;
	}
	return false;
}

FVector ABiodsManager::Seperation(const TArray<FBiod>& NearBiods, FBiod Target)
{
	if(NearBiods.IsEmpty()) return FVector(0);
	FVector Temp(0);
	for (auto it : NearBiods)
	{
		Temp += it.Location;
	}
	return  ((Target.Location-Temp/NearBiods.Num()).GetSafeNormal()*(SeparationRadius-(Target.Location- Temp / NearBiods.Num()).Length()))* (SeparationRadius - (Target.Location - Temp / NearBiods.Num()).Length());
}

FVector ABiodsManager::Alignment(const TArray<FBiod>& NearBiods, FBiod Target)
{
	if(NearBiods.IsEmpty()) return FVector(0);
	FVector Temp(0);
	for (auto it : NearBiods)
	{
		Temp+=it.Velocity;
	}
	return (Temp / NearBiods.Num()-Target.Velocity);
}

FVector ABiodsManager::Cohesion(const TArray<FBiod>& NearBiods, FBiod Target)
{
	if(NearBiods.IsEmpty()) return FVector(0);
	FVector Temp(0);
	for (auto it : NearBiods)
	{
		Temp+=it.Location;
	}
	return (( Temp / NearBiods.Num()- Target.Location).GetSafeNormal() * (Temp / NearBiods.Num()- Target.Location).Length()) * (Temp / NearBiods.Num()-Target.Location).Length();
}

FVector ABiodsManager::Disrupt(FBiod Target)
{
	int32 seed=(int32)(Target.RandSeed*(MoveDirect.X+MoveDirect.Y+MoveDirect.Z))%10000000;
	FVector Temp;
	RandSteam.Initialize(seed);
	Temp.X=RandSteam.FRandRange(-MovePointRandRange, MovePointRandRange);
	Temp.Y=RandSteam.FRandRange(-MovePointRandRange, MovePointRandRange);
	Temp.Z=RandSteam.FRandRange(-MovePointRandRange, MovePointRandRange);
	return Temp;
}

void ABiodsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickBiods(DeltaTime);
}

