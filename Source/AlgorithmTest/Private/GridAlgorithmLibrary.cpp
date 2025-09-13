// Fill out your copyright notice in the Description page of Project Settings.


#include "GridAlgorithmLibrary.h"


TArray<AGridActor*> UGridDataManager::Retrieve_Implementation(FBox2D Box)
{
	TArray<AGridActor*> Temp;
	int32 BoxMinX=FMath::Max((Box.Min.X-AreaMin.X)/GridSize-1,0);
	int32 BoxMaxX=FMath::Min((Box.Max.X- AreaMin.X)/GridSize+1,GridXcount);
	int32 BoxMinY= FMath::Max((Box.Min.Y-AreaMin.Y) / GridSize - 1, 0);
	int32 BoxMaxY= FMath::Min((Box.Max.Y- AreaMin.Y) / GridSize + 1, GridYcount);
	for (int32 i = BoxMinX; i <= BoxMaxX; i++)
	{
		for (int32 j = BoxMinY; j <= BoxMaxY; j++)
		{
			Temp.Append(DataArray[i][j]);
		}
	}
	TSet<AGridActor*> TempSet(Temp);
	return TempSet.Array();
}

bool UGridDataManager::Insert_Implementation(AGridActor* GridActor)
{
	bool Temp=false;
	int32 ActorMinX=(GridActor->Size.Min.X- AreaMin.X)/GridSize;
	int32 ActorMaxX=(GridActor->Size.Max.X- AreaMin.X)/GridSize;
	int32 ActorMinY=(GridActor->Size.Min.Y- AreaMin.Y)/GridSize;
	int32 ActorMaxY=(GridActor->Size.Max.Y- AreaMin.Y)/GridSize;
	for (int32 i = ActorMinX; i <= ActorMaxX; i++)
	{
		for (int32 j = ActorMinY; j <= ActorMaxY; j++)
		{
			if (!DataArray[i][j].Contains(GridActor))
			{
				Temp=true;
				DataArray[i][j].Add(GridActor);
				GridActor->OwnGrid.Add(FVector2D(i,j));
				GridActor->Manager=this;
			}

		}
	}
	return Temp;
}

bool UGridDataManager::Remove_Implementation(AGridActor* GridActor)
{
	bool Temp = false;
	int32 ActorMinX = (GridActor->Size.Min.X - AreaMin.X)/ GridSize;
	int32 ActorMaxX = (GridActor->Size.Max.X - AreaMin.X)/ GridSize;
	int32 ActorMinY = (GridActor->Size.Min.Y - AreaMin.Y)/ GridSize;
	int32 ActorMaxY = (GridActor->Size.Max.Y - AreaMin.Y)/ GridSize;
	for (int32 i = ActorMinX; i <= ActorMaxX; i++)
	{
		for (int32 j = ActorMinY; j <= ActorMaxY; j++)
		{
			if (DataArray[i][j].Contains(GridActor))
			{
				Temp = true;
				DataArray[i][j].Remove(GridActor);
				GridActor->OwnGrid.Remove(FVector2D(i,j));
			}

		}
	}
	return Temp;
}

bool UGridDataManager::MoveActor_Implementation(AGridActor* GridActor, FVector2D NewLocation)
{
	Remove(GridActor);
	GridActor->Size.Min = FVector2D(NewLocation.X-GridActor->GetActorLocation().X+GridActor->Size.Min.X,GridActor->Size.Min.Y+NewLocation.Y-GridActor->GetActorLocation().Y);
	GridActor->Size.Max = FVector2D(NewLocation.X - GridActor->GetActorLocation().X + GridActor->Size.Max.X, GridActor->Size.Max.Y + NewLocation.Y - GridActor->GetActorLocation().Y);
	GridActor->SetActorLocation(FVector(NewLocation.X,NewLocation.Y,GridActor->GetActorLocation().Z));
	Insert(GridActor);
	return true;

}

bool UGridDataManager::SpawnRandomPoint_Implementation(float BoxSize, FBox2D& NewBox)
{
	uint64 Nanoseconds = FPlatformTime::Cycles64();
	int32 Seed = static_cast<int32>(Nanoseconds % INT32_MAX);
	FRandomStream RandomStream(Seed);
	FVector2D Min(AreaMin.X,AreaMin.Y);
	FVector2D Max(AreaMax.X,AreaMax.Y);
	int32 Count=0;
	FVector2D NewLocation= FVector2D(RandomStream.FRandRange(Min.X,Max.X),RandomStream.FRandRange(Min.Y,Max.Y));
	NewBox=FBox2D(FVector2D(NewLocation.X-BoxSize,NewLocation.Y-BoxSize),FVector2D(NewLocation.X+BoxSize,NewLocation.Y+BoxSize));
	while (IsOverlap(NewBox))
	{
		if(Count>200)//寻找次数，超过200次认为已无可生成点
		{
			NewBox = FBox2D(0, 0);
			return false;
		}
		NewLocation = FVector2D(RandomStream.FRandRange(Min.X, Max.X), RandomStream.FRandRange(Min.Y, Max.Y));
		NewBox =FBox2D(FVector2D(NewLocation.X - BoxSize, NewLocation.Y - BoxSize), FVector2D(NewLocation.X + BoxSize, NewLocation.Y + BoxSize));
		Count++;
	}
	return true;
}

void UGridDataManager::Init()
{
	GridXcount=(AreaMax.X-AreaMin.X)/GridSize;
	GridYcount=(AreaMax.Y-AreaMin.Y)/GridSize;
	DataArray.SetNum(GridXcount+1);
	for (int32 i = 0; i <= GridXcount; i++)
	{
		DataArray[i].SetNum(GridYcount+1);
	}
}

bool UGridDataManager::IsOverlap_Implementation(FBox2D Size)
{
	TArray<AGridActor*> TempActor=Retrieve(Size);
	if(TempActor.IsEmpty())
		return false;
	for (auto it : TempActor)
	{
		if (Size.Intersect(it->Size))
			return true;
	}
	return false;
}

void AGridActor::Activate_Implementation()
{

}

void AGridActor::Deactivate_Implementation()
{

}

TArray<AGridActor*> UHashGridDataManger::Retrieve_Implementation(FBox2D Box)
{
	TArray<AGridActor*> Temp;
	int32 BoxMinX = FMath::Max((Box.Min.X - AreaMin.X) / GridSize - 1, 0);
	int32 BoxMaxX = FMath::Min((Box.Max.X - AreaMin.X) / GridSize + 1, GridXcount);
	int32 BoxMinY = FMath::Max((Box.Min.Y - AreaMin.Y) / GridSize - 1, 0);
	int32 BoxMaxY = FMath::Min((Box.Max.Y - AreaMin.Y) / GridSize + 1, GridYcount);
	for (int32 i = BoxMinX; i <= BoxMaxX; i++)
	{
		for (int32 j = BoxMinY; j <= BoxMaxY; j++)
		{
			int32 key = calculateHashKey(i, j, 0);
			int32 index = abs(key) & (Table_size-1);
			for (auto it : HashBuckets[index])
			{
				//if(IsOnTargetGrid(FVector2D(i,j),it->Size))
				if(it->OwnGrid.Contains(FVector2D(i, j)))
					Temp.Add(it);
			}
		}
	}
	TSet<AGridActor*> TempSet(Temp);
	return TempSet.Array();
}

bool UHashGridDataManger::Insert_Implementation(AGridActor* GridActor)
{
	bool Temp = false;

	//得到Actor对应的网格
	int32 ActorMinX = (GridActor->Size.Min.X - AreaMin.X) / GridSize;
	int32 ActorMaxX = (GridActor->Size.Max.X - AreaMin.X) / GridSize;
	int32 ActorMinY = (GridActor->Size.Min.Y - AreaMin.Y) / GridSize;
	int32 ActorMaxY = (GridActor->Size.Max.Y - AreaMin.Y) / GridSize;
	//对所在网格进行业务操作
	for (int32 i = ActorMinX; i <= ActorMaxX; i++)
	{
		for (int32 j = ActorMinY; j <= ActorMaxY; j++)
		{
			//计算哈希
			int32 key = calculateHashKey(i, j, 0);
			int32 index = abs(key) & (Table_size-1);
			if (index >= Table_size)
			{
				HashBuckets.SetNum(Table_size++);
			}
			//业务逻辑
			if(!HashBuckets[index].Contains(GridActor))
			{
				HashBuckets[index].Add(GridActor);
				GridActor->OwnGrid.Add(FVector2D(i,j));
				GridActor->Manager = this;
				Temp=true;
			}
		}
	}
	return Temp;
}

bool UHashGridDataManger::Remove_Implementation(AGridActor* GridActor)
{
	bool Temp = false;
	int32 ActorMinX = (GridActor->Size.Min.X - AreaMin.X) / GridSize;
	int32 ActorMaxX = (GridActor->Size.Max.X - AreaMin.X) / GridSize;
	int32 ActorMinY = (GridActor->Size.Min.Y - AreaMin.Y) / GridSize;
	int32 ActorMaxY = (GridActor->Size.Max.Y - AreaMin.Y) / GridSize;
	for (int32 i = ActorMinX; i <= ActorMaxX; i++)
	{
		for (int32 j = ActorMinY; j <= ActorMaxY; j++)
		{
			int32 key = calculateHashKey(i, j, 0);
			int32 index = abs(key) & (Table_size-1);
			if (index >= Table_size)
			{
				return false;
			}
			if (HashBuckets[index].Contains(GridActor))
			{
				HashBuckets[index].Remove(GridActor);
				GridActor->OwnGrid.Remove(FVector2D(i, j));
				Temp = true;
			}
		}
	}
	return Temp;
}

bool UHashGridDataManger::MoveActor_Implementation(AGridActor* GridActor, FVector2D NewLocation)
{
	Remove(GridActor);
	GridActor->Size.Min = FVector2D(NewLocation.X - GridActor->GetActorLocation().X + GridActor->Size.Min.X, GridActor->Size.Min.Y + NewLocation.Y - GridActor->GetActorLocation().Y);
	GridActor->Size.Max = FVector2D(NewLocation.X - GridActor->GetActorLocation().X + GridActor->Size.Max.X, GridActor->Size.Max.Y + NewLocation.Y - GridActor->GetActorLocation().Y);
	GridActor->SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GridActor->GetActorLocation().Z));
	Insert(GridActor);
	return true;
}

bool UHashGridDataManger::SpawnRandomPoint_Implementation(float BoxSize, FBox2D& NewBox)
{
	uint64 Nanoseconds = FPlatformTime::Cycles64();
	int32 Seed = static_cast<int32>(Nanoseconds % INT32_MAX);
	FRandomStream RandomStream(Seed);
	FVector2D Min(AreaMin.X, AreaMin.Y);
	FVector2D Max(AreaMax.X, AreaMax.Y);
	int32 Count = 0;
	FVector2D NewLocation = FVector2D(RandomStream.FRandRange(Min.X, Max.X), RandomStream.FRandRange(Min.Y, Max.Y));
	NewBox = FBox2D(FVector2D(NewLocation.X - BoxSize, NewLocation.Y - BoxSize), FVector2D(NewLocation.X + BoxSize, NewLocation.Y + BoxSize));
	while (IsOverlap(NewBox))
	{
		if (Count > 200)//寻找次数，超过200次认为已无可生成点
		{
			NewBox = FBox2D(0, 0);
			return false;
		}
		NewLocation = FVector2D(RandomStream.FRandRange(Min.X, Max.X), RandomStream.FRandRange(Min.Y, Max.Y));
		NewBox = FBox2D(FVector2D(NewLocation.X - BoxSize, NewLocation.Y - BoxSize), FVector2D(NewLocation.X + BoxSize, NewLocation.Y + BoxSize));
		Count++;
	}
	return true;
}

bool UHashGridDataManger::IsOverlap_Implementation(FBox2D Size)
{
	TArray<AGridActor*> TempActor = Retrieve(Size);
	if (TempActor.IsEmpty())
		return false;
	for (auto it : TempActor)
	{
		if (Size.Intersect(it->Size))
			return true;
	}
	return false;
}


void UHashGridDataManger::Init()
{
	GridXcount = (AreaMax.X - AreaMin.X) / GridSize;
	GridYcount = (AreaMax.Y - AreaMin.Y) / GridSize;
	HashBuckets.SetNum(Table_size);
}

void UHashGridDataManger::Clear()
{
	for (auto it : HashBuckets)
	{
		it.Empty();
	}
}

int32 UHashGridDataManger::calculateHashKey(int32 x, int32 y, int32 z)
{
	return (x * 73856093) ^ (y * 19349663) ^ (z * 83492791);
}



USpaceGrid* USpaceGrid::CreateSpaceGrid(UObject* WorldContextObject, TSubclassOf<USpaceGrid> Class, FVector2D _AreaMin, FVector2D _AreaMax, int32 _GridSize /*= 10000*/)
{
	USpaceGrid* Temp = NewObject<USpaceGrid>(WorldContextObject,Class);
	Temp->AreaMin = _AreaMin;
	Temp->AreaMax = _AreaMax;
	Temp->GridSize = _GridSize;
	Temp->Init();
	return Temp;
}

TArray<AGridActor*> USpaceGrid::Retrieve(FBox2D Box)
{
	return Retrieve_Implementation(Box);
}

TArray<AGridActor*> USpaceGrid::Retrieve_Implementation(FBox2D Box)
{
	TArray<AGridActor*> Temp;
	return Temp;
}

bool USpaceGrid::Insert(AGridActor* GridActor)
{
	return Insert_Implementation(GridActor);
}

bool USpaceGrid::Insert_Implementation(AGridActor* GridActor)
{
	return false;
}

bool USpaceGrid::Remove(AGridActor* GridActor)
{
	return Remove_Implementation(GridActor);
}

bool USpaceGrid::Remove_Implementation(AGridActor* GridActor)
{
	return false;
}

bool USpaceGrid::MoveActor(AGridActor* GridActor, FVector2D NewLocation)
{
	return MoveActor_Implementation(GridActor,NewLocation);
}

bool USpaceGrid::MoveActor_Implementation(AGridActor* GridActor, FVector2D NewLocation)
{
	return false;
}

bool USpaceGrid::SpawnRandomPoint(float BoxSize, FBox2D& NewBox)
{
	return SpawnRandomPoint_Implementation(BoxSize,NewBox);
}

bool USpaceGrid::SpawnRandomPoint_Implementation(float BoxSize, FBox2D& NewBox)
{
	return false;
}

bool USpaceGrid::IsOverlap(FBox2D Size)
{
	return IsOverlap_Implementation(Size);
}

bool USpaceGrid::IsOverlap_Implementation(FBox2D Size)
{
	return false;
}

void USpaceGrid::Init()
{

}

void USpaceGrid::Clear()
{

}
