// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/Triggers/FlowTriggerVolume.h"

#include "Flow/Triggers/FlowTriggerComponent.h"

#include "Components/BrushComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowTriggerVolume)

AFlowTriggerVolume::AFlowTriggerVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static FName CollisionProfileName(TEXT("Trigger"));
	GetBrushComponent()->SetCollisionProfileName(CollisionProfileName);

	bColored = true;
	BrushColor.R = 100;
	BrushColor.G = 255;
	BrushColor.B = 100;
	BrushColor.A = 255;

	bReplicates = true;
	FlowTriggerComponent = CreateDefaultSubobject<UFlowTriggerComponent>(TEXT("FlowTriggerComponent"));
}
