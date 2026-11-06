// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/Nodes/FlowNode_SpawnByGameplayTag.h"

#include "FlowSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_SpawnByGameplayTag)

UFlowNode_SpawnByGameplayTag::UFlowNode_SpawnByGameplayTag()
{
#if WITH_EDITOR
	Category = TEXT("Actor");
#endif

	InputPins = {FFlowPin(TEXT("Spawn")), FFlowPin(TEXT("Despawn"))};
	OutputPins = {FFlowPin(TEXT("Spawned")), FFlowPin(TEXT("Despawned"))};
}

void UFlowNode_SpawnByGameplayTag::ExecuteInput(const FName& PinName)
{
	if (IdentityTags.IsValid())
	{
		if (PinName == TEXT("Spawn"))
		{
			SpawnActors(false);
		}
		else if (PinName == TEXT("Despawn"))
		{
			for (TWeakObjectPtr<USpawnComponent> SpawnComponent : SpawnComponents)
			{
				if (SpawnComponent.IsValid())
				{
					SpawnComponent->Despawn();
				}
			}
		}
	}
	else
	{
		LogError(MissingIdentityTag);
	}

	// Graph continues also if no actor was spawned or despawned
	if (PinName == TEXT("Spawn"))
	{
		TriggerOutput(TEXT("Spawned"));
	}
	else if (PinName == TEXT("Despawn"))
	{
		TriggerOutput(TEXT("Despawned"), true);
	}
}

bool UFlowNode_SpawnByGameplayTag::CanFastForward(const FName& InputPinName, const FName& OutputPinName) const
{
	return (InputPinName == TEXT("Despawn")) == (OutputPinName == TEXT("Despawned"));
}

void UFlowNode_SpawnByGameplayTag::Cleanup()
{
	SpawnComponents.Empty();

	Super::Cleanup();
}

void UFlowNode_SpawnByGameplayTag::OnLoad_Implementation()
{
	// Spawned actors aren't saved, so the active node spawns them again
	// Game loaded in the same world still has actors spawned before
	if (IdentityTags.IsValid())
	{
		SpawnActors(true);
	}
}

void UFlowNode_SpawnByGameplayTag::SpawnActors(const bool bKeepSpawnedActors)
{
	for (const TWeakObjectPtr<UFlowComponent>& FoundComponent : GetFlowSubsystem()->GetComponents<UFlowComponent>(IdentityTags, EGameplayContainerMatchType::Any))
	{
		TArray<USpawnComponent*> NewSpawnComponents;
		FoundComponent->GetOwner()->GetComponents<USpawnComponent>(NewSpawnComponents);

		for (USpawnComponent* SpawnComponent : NewSpawnComponents)
		{
			if (!SpawnComponents.Contains(SpawnComponent))
			{
				if ((bKeepSpawnedActors && SpawnComponent->HasSpawnedActor()) || SpawnComponent->Spawn(SpawnParams))
				{
					SpawnComponents.Emplace(SpawnComponent);
				}
			}
		}
	}
}

#if WITH_EDITOR
FString UFlowNode_SpawnByGameplayTag::GetNodeDescription() const
{
	const FString ClassString = SpawnParams.ActorClass ? SpawnParams.ActorClass->GetFName().ToString() : TEXT("Missing Actor Class!");
	return GetIdentityTagsDescription(IdentityTags) + LINE_TERMINATOR + ClassString;
}

EDataValidationResult UFlowNode_SpawnByGameplayTag::ValidateNode()
{
	if (IdentityTags.IsEmpty() || SpawnParams.ActorClass == nullptr)
	{
		if (IdentityTags.IsEmpty())
		{
			ValidationLog.Error<UFlowNode>(*UFlowNode::MissingIdentityTag, this);
		}
		if (SpawnParams.ActorClass == nullptr)
		{
			ValidationLog.Error<UFlowNode>(TEXT("Actor Class is missing or invalid"), this);
		}

		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif
