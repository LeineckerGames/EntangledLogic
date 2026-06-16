#include "XGateFactory.h"
#include "Components/SceneCaptureComponent2D.h"

AXGateFactory::AXGateFactory()
{
	FactoryInsideCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("FactoryInsideCamera"));
	FactoryInsideCamera->SetupAttachment(FactoryMesh);
}