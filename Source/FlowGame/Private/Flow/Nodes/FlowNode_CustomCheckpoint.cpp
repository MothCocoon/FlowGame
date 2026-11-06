// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/Nodes/FlowNode_CustomCheckpoint.h"

#include "Flow/FlowSaveSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_CustomCheckpoint)

UFlowNode_CustomCheckpoint::UFlowNode_CustomCheckpoint()
{
#if WITH_EDITOR
	Category = TEXT("Graph");
#endif
}

void UFlowNode_CustomCheckpoint::ExecuteInput(const FName& PinName)
{
	if (GetWorld())
	{
		if (UFlowSaveSubsystem* SaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UFlowSaveSubsystem>())
		{
			SaveSubsystem->SaveGame();
		}
	}

	TriggerFirstOutput(true);
}

void UFlowNode_CustomCheckpoint::OnLoad_Implementation()
{
	TriggerFirstOutput(true);
}
