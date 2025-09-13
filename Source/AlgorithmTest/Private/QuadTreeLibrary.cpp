// Fill out your copyright notice in the Description page of Project Settings.


#include "QuadTreeLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "QuadTreePointInterface.h"

UOcTreeNode* UOcTreeNode::CreateOcTreeNode(FVector NodeLocation, FVector NodeRange, int32 MaxLevel, int32 _SplitCount)
{
	UOcTreeNode* Node=NewObject<UOcTreeNode>();
	Node->max_Level= MaxLevel;
	Node->SplitCount=_SplitCount;
	Node->Init(0,NodeLocation,NodeRange);
	return Node;
}

bool UOcTreeNode::Insert(AOcTreePoint* NewPoint)
{
	if (!IsInRange(NewPoint->GetLocation(),NewPoint->GetRange()))
		return false;
	bool Tempbool=false;
	if (ChildNodes.Num() == 0)
	{
		Points.Add(NewPoint);
		if(!NewPoint->OcTreeArea.Contains(this))
			NewPoint->OcTreeArea.Add(this);
		TrySplit();
		return true;
	}
	else 
	{
		TArray<int32> tempchilds= GetIndex(NewPoint->GetLocation(), NewPoint->GetRange());
		for (int32 i = 0; i < tempchilds.Num(); i++)
		{
			if(ChildNodes[tempchilds[i]]->Insert(NewPoint))
				Tempbool=true;
		}
	}
	return Tempbool;
}

//bool UOcTreeNode::Insert(AOcTreePoint* NewPoint)
//{
//	if(!IsInRange(NewPoint->GetLocation()))
//	return false;
//
//	if (ChildNodes.Num() == 0)
//	{
//		Points.Add(NewPoint);
//		TrySplit();
//	}
//	else
//	{
//		int32 index=GetIndex(NewPoint->GetLocation(),NewPoint->GetRange());
//		if (index == -1)
//		{
//			Points.Add(NewPoint);
//			DirectlyPointCount++;
//		}
//		else
//		{
//			ChildNodes[index]->Insert(NewPoint);
//		}
//	}
//	return true;
//}

void UOcTreeNode::Clear()
{
	for (UOcTreeNode* it : ChildNodes)
	{
		it->Clear();
	}
	for (AOcTreePoint* it : Points)
	{
		it->DeletePoint();
	}
	ChildNodes.Empty();
	Points.Empty();
}

//TArray<AOcTreePoint*> UOcTreeNode::Retrieve(const FVector PointLocation, const float PointRange)
//{
//	TArray<AOcTreePoint*> TempPoints;
//	if(!IsInRange(PointLocation))
//	return TempPoints;
//
//	if(ChildNodes.Num()==0)
//	return Points;
//
//	if (GetIndex(PointLocation, PointRange) == -1)
//	{
//		TempPoints.Append(Points);
//		if ((PointLocation.X + PointRange) > Location.X)
//		{
//			if ((PointLocation.Y + PointRange) > Location.Y)
//			{
//				if ((PointLocation.Z + PointRange) > Location.Z)
//				{
//					TempPoints.Append(ChildNodes[0]->Retrieve(PointLocation, PointRange));
//				}
//				if ((PointLocation.Z - PointRange) < Location.Z)
//				{
//					TempPoints.Append(ChildNodes[1]->Retrieve(PointLocation, PointRange));
//				}
//			}
//			if ((PointLocation.Y - PointRange) < Location.Y)
//			{
//				if ((PointLocation.Z + PointRange) > Location.Z)
//				{
//					TempPoints.Append(ChildNodes[2]->Retrieve(PointLocation, PointRange));
//				}
//				if ((PointLocation.Z - PointRange) < Location.Z)
//				{
//					TempPoints.Append(ChildNodes[3]->Retrieve(PointLocation, PointRange));
//				}
//			}
//		}
//		if ((PointLocation.X - PointRange) < Location.X)
//		{
//			if ((PointLocation.Y + PointRange) > Location.Y)
//			{
//				if ((PointLocation.Z + PointRange) > Location.Z)
//				{
//					TempPoints.Append(ChildNodes[4]->Retrieve(PointLocation, PointRange));
//				}
//				if ((PointLocation.Z - PointRange) < Location.Z)
//				{
//					TempPoints.Append(ChildNodes[5]->Retrieve(PointLocation, PointRange));
//				}
//			}
//			if ((PointLocation.Y - PointRange) < Location.Y)
//			{
//				if ((PointLocation.Z + PointRange) > Location.Z)
//				{
//					TempPoints.Append(ChildNodes[6]->Retrieve(PointLocation, PointRange));
//				}
//				if ((PointLocation.Z - PointRange) < Location.Z)
//				{
//					TempPoints.Append(ChildNodes[7]->Retrieve(PointLocation, PointRange));
//				}
//			}
//		}
//	}
//	else
//	{
//		TempPoints.Append(ChildNodes[GetIndex(PointLocation, PointRange)]->Retrieve(PointLocation, PointRange));
//	}
//	return TempPoints;
//}

TArray<AOcTreePoint*> UOcTreeNode::Retrieve(const FVector PointLocation, const float PointRange)
{
	TArray<AOcTreePoint*> TempPoints;
	if (!IsInRange(PointLocation,PointRange))
		return TempPoints;

	if (ChildNodes.Num() == 0)
		return Points;
	TArray<int32> TempArr=GetIndex(PointLocation,PointRange);
	for (int32 i = 0; i < TempArr.Num(); i++)
	{
		TempPoints.Append(ChildNodes[TempArr[i]]->Retrieve(PointLocation,PointRange));
	}
	return TempPoints;
}

bool UOcTreeNode::TryRemoveAt(const FVector PointLocation, const float PointRange)
{
	if (!IsInRangeOuter(PointLocation,PointRange))
		return false;
	bool Tempbool=false;
	if (ChildNodes.Num() == 0)
	{
		for (int32 it=0;it<Points.Num() ; it++)
		{
			if (Points[it]->GetLocation() == PointLocation)
			{
				RemoveAt(it);
				return true;
			}
		}
		return false;
	}
	TArray<int32> indexarr = GetIndex(PointLocation, PointRange);
	for (int32 i = 0; i < indexarr.Num(); i++)
	{
		if (ChildNodes[indexarr[i]]->TryRemoveAt(PointLocation, PointRange))
		{
			Tempbool=true;
		}
	}
	return Tempbool;
	//int32 index=GetIndex(PointLocation, PointRange);
	//if (index==-1)
	//{
	//	for (int32 it = 0; it < Points.Num(); it++)
	//	{
	//		if (Points[it]->GetLocation() == PointLocation)
	//		{
	//			Remove(it);
	//			return true;
	//		}
	//	}
	//}
	//else
	//{
	//	return ChildNodes[index]->TryRemove(PointLocation, PointRange);
	//}
	//return false;
}

bool UOcTreeNode::TryRemove(AOcTreePoint* Point)
{
	if (!IsInRangeOuter(Point->GetLocation(),Point->GetRange()))
		return false;
	bool Tempbool = false;
	if (ChildNodes.Num() == 0)
	{
		if (Points.Contains(Point))
		{
			Remove(Point);
			return true;
		}
		return false;
	}
	TArray<int32> indexarr = GetIndex(Point->GetLocation(), Point->GetRange());
	for (int32 i = 0; i < indexarr.Num(); i++)
	{
		if (ChildNodes[indexarr[i]]->TryRemove(Point))
		{
			Tempbool = true;
		}
	}
	return Tempbool;
}

TArray<FVector> UOcTreeNode::GetAllLocation()
{
	TArray<FVector> TempLocation;
	TempLocation.Add(Location);
	for (auto it : ChildNodes)
	{
		TempLocation.Append(it->GetAllLocation());
	}
	return TempLocation;
}

TArray<FVector> UOcTreeNode::GetAllRange()
{
	TArray<FVector> TempRange;
	TempRange.Add(Range);
	for (auto it : ChildNodes)
	{
		TempRange.Append(it->GetAllRange());
	}
	return TempRange;
}

bool UOcTreeNode::SpawnRandOcTreePoint(float PointRange, FVector& NewLocation)
{
	FBox TempBox;
	int32 findCount=0;
	TempBox.Max=Location+Range;
	TempBox.Min=Location-Range;
	NewLocation= FMath::RandPointInBox(TempBox);
	while (IsOverlap(NewLocation, PointRange))
	{
		if (findCount > 200)//寻找次数，超过200次认为已无可生成点
		{
			 return false;
		}
		NewLocation = FMath::RandPointInBox(TempBox);
		findCount++;
		
	}
	return true;
}

bool UOcTreeNode::IsOverlap(const FVector NewLocation, const float NewRange)
{
	TArray<AOcTreePoint*> Temp=Retrieve(NewLocation,NewRange);
	if(Temp.IsEmpty())
		return false;
	for (auto it : Temp)
	{
		if(UKismetMathLibrary::Vector_Distance(NewLocation,it->GetLocation())<(NewRange+it->GetRange()))
			return true;
	}
	return false;
}

bool UOcTreeNode::MoveNode(AOcTreePoint* Point, FVector NewLocation)
{
	TArray<UOcTreeNode*> NewArea=GetContainNode(Point, NewLocation);
	TArray<UOcTreeNode*> toRemove,toAdd;

	for (UOcTreeNode* Node : Point->OcTreeArea)
	{
		if (!NewArea.Contains(Node))
		{
			toRemove.Add(Node);
		}
	}
	for (UOcTreeNode* Node : NewArea)
	{
		if (!Point->OcTreeArea.Contains(Node))
		{
			toAdd.Add(Node);
		}
	}
	for (UOcTreeNode* Node : toRemove)
	{
		if(Node->Points.Contains(Point))
		{
			Node->TryRemove(Point);
		}
	}
	Point->SetLocation(NewLocation);
	for (UOcTreeNode* Node : toAdd)
	{
		if(!Node->Points.Contains(Point))
		{ 
			Node->Insert(Point);
		}
	}
	return true;
}

void UOcTreeNode::RemoveAt(int32 PointIndex)
{
	if (Points[PointIndex]->OcTreeArea.Contains(this))
		Points[PointIndex]->OcTreeArea.Remove(this);
	Points.RemoveAt(PointIndex);
	TryMerge();
}

void UOcTreeNode::Remove(AOcTreePoint* Point)
{
	if(Point->OcTreeArea.Contains(this))
		Point->OcTreeArea.Remove(this);
	Points.Remove(Point);
	TryMerge();
}

void UOcTreeNode::Split()
{
	ChildNodes.Empty();
	UOcTreeNode* TFR= NewObject<UOcTreeNode>();
	TFR->Init(Cur_Level+1,Location+Range/2,Range/2);
	ChildNodes.Add(TFR);
	UOcTreeNode* TFL=NewObject<UOcTreeNode>();
	TFL->Init(Cur_Level+1,FVector(Location.X+Range.X/2,Location.Y-Range.Y/2,Location.Z+Range.Z/2),Range/2);
	ChildNodes.Add(TFL);
	UOcTreeNode* TBR=NewObject<UOcTreeNode>();
	TBR->Init(Cur_Level+1,FVector(Location.X-Range.X/2,Location.Y+Range.Y/2,Location.Z+Range.Z/2),Range/2);
	ChildNodes.Add(TBR);
	UOcTreeNode* TBL = NewObject<UOcTreeNode>();
	TBL->Init(Cur_Level + 1, FVector(Location.X - Range.X / 2, Location.Y - Range.Y / 2, Location.Z + Range.Z / 2), Range / 2);
	ChildNodes.Add(TBL);
	UOcTreeNode* DFR = NewObject<UOcTreeNode>();
	DFR->Init(Cur_Level + 1, FVector(Location.X + Range.X / 2, Location.Y + Range.Y / 2, Location.Z - Range.Z / 2), Range / 2);
	ChildNodes.Add(DFR);
	UOcTreeNode* DFL = NewObject<UOcTreeNode>();
	DFL->Init(Cur_Level + 1, FVector(Location.X + Range.X / 2, Location.Y - Range.Y / 2, Location.Z - Range.Z / 2), Range / 2);
	ChildNodes.Add(DFL);
	UOcTreeNode* DBR = NewObject<UOcTreeNode>();
	DBR->Init(Cur_Level + 1, FVector(Location.X - Range.X / 2, Location.Y + Range.Y / 2, Location.Z - Range.Z / 2), Range / 2);
	ChildNodes.Add(DBR);
	UOcTreeNode* DBL = NewObject<UOcTreeNode>();
	DBL->Init(Cur_Level + 1, Location - Range / 2, Range / 2);
	ChildNodes.Add(DBL);

	//TArray<AOcTreePoint*> NewPoints;
	for (AOcTreePoint* it:Points)
	{
		//int32 index= GetIndex(it->GetLocation(), it->GetRange());
		//if (index !=-1)
		//{
		//	ChildNodes[index]->Insert(it);
		//}
		//else
		//{
		//	NewPoints.Add(it);
		//}
		it->OcTreeArea.Empty();
		TArray<int32> indexarr=GetIndex(it->GetLocation(),it->GetRange());
		for (int32 i : indexarr)
		{
			ChildNodes[i]->Insert(it);
		}
	}
	Points.Empty();
	//Points=NewPoints;
}

bool UOcTreeNode::TrySplit()
{
	if((Points.Num()-DirectlyPointCount)>SplitCount)
	{	
		Split();
		return true;
	}
	return false;
}

bool UOcTreeNode::TryMerge()
{
	bool IsMerge=true;
	int32 ChildPointCount=0;
	for (auto it : ChildNodes)
	{
		if (it->ChildNodes.Num() != 0)
		{
			IsMerge=false;
			it->TryMerge();
		}
		else {
			ChildPointCount+=it->Points.Num();
		}
	}
	if(IsMerge&&ChildPointCount<(SplitCount/2))
		Merge();

	return false;
}

void UOcTreeNode::Merge()
{
	for (auto it : ChildNodes)
	{
		if(!it->Points.IsEmpty())
			Points.Append(it->Points);
		it->ConditionalBeginDestroy();
	}
	ChildNodes.Empty();
	
}

TArray<int32> UOcTreeNode::GetIndex(FVector PointLocation, float PointRange)
{
	TArray<int32> Temp;
	if ((PointLocation.X + PointRange) > Location.X)
	{
		if ((PointLocation.Y + PointRange) > Location.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z)
			{
				Temp.Add(0);
			}
			if ((PointLocation.Z - PointRange) < Location.Z)
			{
				Temp.Add(4);
			}
		}
		if ((PointLocation.Y - PointRange) < Location.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z)
			{
				Temp.Add(1);
			}
			if ((PointLocation.Z - PointRange) < Location.Z)
			{
				Temp.Add(5);
			}
		}
	}
	if ((PointLocation.X - PointRange) < Location.X)
	{
		if ((PointLocation.Y + PointRange) > Location.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z)
			{
				Temp.Add(2);
			}
			if ((PointLocation.Z - PointRange) < Location.Z)
			{
				Temp.Add(6);
			}
		}
		if ((PointLocation.Y - PointRange) < Location.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z)
			{
				Temp.Add(3);
			}
			if ((PointLocation.Z - PointRange) < Location.Z)
			{
				Temp.Add(7);
			}
		}
	}
	return Temp;
}

TArray<int32> UOcTreeNode::GetIndexbyloose(FVector PointLocation, float PointRange)
{
	TArray<int32> Temp;
	if ((PointLocation.X + PointRange) > Location.X - Range.X)
	{
		if ((PointLocation.Y + PointRange) > Location.Y - Range.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z - Range.Z)
			{
				Temp.Add(0);
			}
			if ((PointLocation.Z - PointRange) < Location.Z + Range.Z)
			{
				Temp.Add(4);
			}
		}
		if ((PointLocation.Y - PointRange) < Location.Y + Range.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z - Range.Z)
			{
				Temp.Add(1);
			}
			if ((PointLocation.Z - PointRange) < Location.Z + Range.Z)
			{
				Temp.Add(5);
			}
		}
	}
	if ((PointLocation.X - PointRange) < Location.X + Range.X)
	{
		if ((PointLocation.Y + PointRange) > Location.Y - Range.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z - Range.Z)
			{
				Temp.Add(2);
			}
			if ((PointLocation.Z - PointRange) < Location.Z + Range.Z)
			{
				Temp.Add(6);
			}
		}
		if ((PointLocation.Y - PointRange) < Location.Y + Range.Y)
		{
			if ((PointLocation.Z + PointRange) > Location.Z - Range.Z)
			{
				Temp.Add(3);
			}
			if ((PointLocation.Z - PointRange) < Location.Z + Range.Z)
			{
				Temp.Add(7);
			}
		}
	}
	return Temp;
}

TArray<UOcTreeNode*> UOcTreeNode::GetContainNode(AOcTreePoint* Point, FVector NewLocation)
{
	TArray<UOcTreeNode*> TempArray;
	FBox NodeBox(Location-RangeOuter, Location+RangeOuter);
	FBox PointBox(NewLocation -Point->GetRange(), NewLocation +Point->GetRange());
	if(!NodeBox.Intersect(PointBox))
		return TempArray;
	if (ChildNodes.IsEmpty())
	{
		TempArray.Add(this);
		return TempArray;
	}
	for (auto it :ChildNodes)
	{
		TempArray.Append(it->GetContainNode(Point, NewLocation));
	}
	return TempArray;
}

//int32 UOcTreeNode::GetIndex(FVector PointLocation, float PointRange)
//{
//	//检查是否与节点边界相切
//	FBox TempBox;
//	TempBox.Min=Location-Range;
//	TempBox.Max=Location+Range;
//	FVector PointNewRange=FVector(PointRange);
//	FBox PointBox;
//	PointBox.Min=PointLocation-PointNewRange;
//	PointBox.Max=PointLocation+PointNewRange;
//	if (TempBox.IsInsideOrOn(PointBox))
//	{
//		if (!TempBox.IsInside(PointBox))
//		{
//			return -1;
//		}
//		if (UKismetMathLibrary::Abs(PointLocation.X - Location.X) < PointRange || UKismetMathLibrary::Abs(PointLocation.Y - Location.Y) < PointRange || UKismetMathLibrary::Abs(PointLocation.Z - Location.Z) < PointRange)
//			return -1;
//	}
//
//	int32 Index;
//	bool Top,Forward,Right;
//	Top=PointLocation.Z>Location.Z;
//	Forward=PointLocation.X>Location.X;
//	Right=PointLocation.Y>Location.Y;
//	if (Top)
//	{
//		if (Forward)
//		{
//			if(Right) Index=0; //上前右
//			else Index=1; //上前左
//		}
//		else
//		{
//			if(Right) Index=2; //上后右
//			else Index=3; //上后左
//		}
//	}
//	else {
//		if (Forward)
//		{
//			if(Right) Index=4; //下前右
//			else Index=5; //下前左
//		}
//		else
//		{
//			if(Right) Index=6; //下后右
//			else Index=7; //下后左
//		}
//	}
//	return Index;
//}

bool UOcTreeNode::IsInRange(FVector NewLocation,float NewRange)
{
	//检查是否在管理范围内
	FBox TempBox(Location-Range,Location+Range);
	FSphere Sphere(NewLocation,NewRange);
	return FMath::SphereAABBIntersection(Sphere,TempBox);
}

bool UOcTreeNode::IsInRangeOuter(FVector NewLocation, float NewRange)
{
	//检查是否在管理范围内
	FBox TempBox(Location - RangeOuter, Location + RangeOuter);
	FSphere Sphere(NewLocation, NewRange);
	return FMath::SphereAABBIntersection(Sphere, TempBox);
}

void AOcTreePoint::Activate_Implementation()
{

}

void AOcTreePoint::Deactivate_Implementation()
{

}

FVector AOcTreePoint::GetLocation_Implementation()
{
	return GetActorLocation();
}

void AOcTreePoint::SetLocation_Implementation(FVector Location)
{
	SetActorLocation(Location);
}

float AOcTreePoint::GetRange_Implementation()
{
	return Range;
}

void AOcTreePoint::DeletePoint_Implementation()
{
	this->Destroy();
}

