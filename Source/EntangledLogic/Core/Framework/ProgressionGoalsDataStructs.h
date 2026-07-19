#pragma once

#include "QppPlugin.h"
#include "Eigen/Dense"
#include <complex>
#include "ProgressionGoalsDataStructs.generated.h"

enum class EUnlockables : uint8;

UENUM()
enum class EProgressionGoals : uint8
{
	MAIN_Ket_One_State,
	MAIN_Negative_Ket_One_State,
	MAIN_Superpositon_State,
	MAIN_Completed,
	SIDE_Ket_Zero_State,
	SIDE_Completed
};

USTRUCT(BlueprintType)
struct FComplexNumber
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	double RealNumber;

	UPROPERTY(EditAnywhere)
	double ImaginaryNumber;
};

USTRUCT(BlueprintType)
struct FKetWrapper
{
	GENERATED_BODY()
	//std::complex

	// Array size should match this QubitsInSystem^2
	// I dont think theres an easy way to force this in editor
	UPROPERTY(EditAnywhere, Category = "Dimension Size")
	int32 QubitsInSystem;

	UPROPERTY(EditAnywhere, Category = "Vector Values")
	TArray<FComplexNumber> ComplexNumArr;

	qpp::ket ConvertToKet()
	{
		int32 KetDimension = FMath::Pow(2.0f, QubitsInSystem);
		// Create Ket with the right dimension
		qpp::ket NewKet(KetDimension);

		// Convert and Add the complex number to each slot in the vector
		int32 Count = 0;
		for (FComplexNumber UnrealComplexNumber : ComplexNumArr)
		{
			std::complex<double> ComplexNumber(UnrealComplexNumber.RealNumber, UnrealComplexNumber.ImaginaryNumber);
			NewKet(Count) = ComplexNumber;
			Count++;
		}

		std::ostringstream oss1;

		// push qpp's display to the stream
		oss1 << qpp::disp(NewKet) << '\n';
		FString KetInformationString = FString(oss1.str().c_str());
		UE_LOG(LogTemp, Display, TEXT("Converted the Complex Number Arr to : %s"), *KetInformationString);
		return NewKet;
	}

	FString ConvertKetToString(qpp::ket KetToConvert)
	{
		// open string stream
		std::ostringstream oss1;

		// push qpp's display to the stream
		oss1 << qpp::disp(KetToConvert) << '\n';

		// string from stream
		return FString(oss1.str().c_str());
	}

	FString ConvertToString()
	{
		return ConvertKetToString(ConvertToKet());
	}
};

USTRUCT(BlueprintType)
struct FProgressionGoalsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString GoalTitle;

	UPROPERTY(EditAnywhere)
	FString GoalDescription;

	UPROPERTY(EditAnywhere)
	FKetWrapper AcceptedState;

	UPROPERTY(VisibleAnywhere)
	FString AcceptedStateString = "EMPTY";

	UPROPERTY(EditAnywhere)
	int32 RequiredStatesAmount;

	UPROPERTY(EditAnywhere)
	EProgressionGoals NextProgressionGoal;

	UPROPERTY(EditAnywhere)
	TArray<EUnlockables> UnlockablesOnCompletion;

};

USTRUCT(BlueprintType)
struct FProgressionGoal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EProgressionGoals ProgressionGoal;

	UPROPERTY(EditAnywhere)
	FProgressionGoalsData ProgressionGoalsData;

	UPROPERTY(EditAnywhere)
	int32 ProgressionGoalCount = 0;

	qpp::ket GetRequiredKet()
	{
		return ProgressionGoalsData.AcceptedState.ConvertToKet();
	}

	bool operator==(const FProgressionGoal& Other) const
	{
		return this->ProgressionGoal == Other.ProgressionGoal;
	}

	bool operator==(const EProgressionGoals& Other) const
	{
		return this->ProgressionGoal == Other;
	}

};