#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QppPlugin.h"
#include "Eigen/Dense"
#include <complex>
#include "ProgressionGoals.generated.h"


enum class EUnlockables : uint8;

UENUM()
enum class EProgressionGoals : uint8
{
	Ket_One_State,
	Negative_Ket_One_State,
	Superpositon_State,
	Completed
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

};

USTRUCT(BlueprintType)
struct FProgressionGoalsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FKetWrapper AcceptedState;

	UPROPERTY(EditAnywhere)
	int32 RequiredStatesAmount;

	UPROPERTY(EditAnywhere)
	EProgressionGoals NextProgressionGoal;

	UPROPERTY(EditAnywhere)
	TArray<EUnlockables> UnlockablesOnCompletion;

};


UCLASS()
class ENTANGLEDLOGIC_API UProgressionGoals : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<EProgressionGoals, FProgressionGoalsData> ProgressionGoals;

};
