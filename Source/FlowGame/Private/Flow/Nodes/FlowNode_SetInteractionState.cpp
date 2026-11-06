// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/Nodes/FlowNode_SetInteractionState.h"

#include "Gameplay/InteractionComponent.h"

#include "FlowSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_SetInteractionState)

UFlowNode_SetInteractionState::UFlowNode_SetInteractionState()
{
#if WITH_EDITOR
	Category = TEXT("Actor");
#endif

	InputPins = { FFlowPin(TEXT("Enable")), FFlowPin(TEXT("Disable")) };
}

void UFlowNode_SetInteractionState::ExecuteInput(const FName& PinName)
{
	for (const TWeakObjectPtr<UFlowComponent>& FoundComponent : GetFlowSubsystem()->GetComponents<UFlowComponent>(IdentityTags, EGameplayContainerMatchType::Any))
	{
		TArray<UInteractionComponent*> FoundInteractions;
		FoundComponent->GetOwner()->GetComponents<UInteractionComponent>(FoundInteractions);
		if (FoundInteractions.Num() > 0)
		{
			if (PinName == TEXT("Enable"))
			{
				FoundInteractions[0]->Enable();
			}
			else
			{
				FoundInteractions[0]->Disable();
			}
		}
	}

	TriggerFirstOutput(true);
}

#if WITH_EDITOR 
FString UFlowNode_SetInteractionState::GetNodeDescription() const
{
	return GetIdentityTagsDescription(IdentityTags);
}

EDataValidationResult UFlowNode_SetInteractionState::ValidateNode()
{
	if (IdentityTags.IsEmpty())
	{
		ValidationLog.Error<UFlowNode>(*UFlowNode::MissingIdentityTag, this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif
