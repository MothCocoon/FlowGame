// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/Nodes/FlowNode_SpawnByActorReference.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_SpawnByActorReference)

UFlowNode_SpawnByActorReference::UFlowNode_SpawnByActorReference()
{
#if WITH_EDITOR
	Category = TEXT("Actor");
#endif

	InputPins = {FFlowPin(TEXT("Spawn")), FFlowPin(TEXT("Despawn"))};
	OutputPins = {FFlowPin(TEXT("Spawned")), FFlowPin(TEXT("Despawned"))};
}

void UFlowNode_SpawnByActorReference::ExecuteInput(const FName& PinName)
{
	if (SpawnPoints.Num() > 0)
	{
		if (PinName == TEXT("Spawn"))
		{
			int32 SpawnCount = 0;

			for (const TSoftObjectPtr<AActor>& SpawnPoint : SpawnPoints)
			{
				if (const AActor* SpawnActor = SpawnPoint.LoadSynchronous())
				{
					TArray<USpawnComponent*> FoundComponents;
					SpawnActor->GetComponents<USpawnComponent>(FoundComponents);
					for (USpawnComponent* FoundComponent : FoundComponents)
					{
						if (!SpawnComponents.Contains(FoundComponent))
						{
							if (FoundComponent->Spawn(SpawnParams))
							{
								SpawnComponents.Emplace(FoundComponent);
								SpawnCount++;
							}
						}
					}
				}
			}

			if (SpawnCount > 0)
			{
				TriggerOutput(TEXT("Spawned"));
			}
		}
		else if (PinName == TEXT("Despawn"))
		{
			int32 DespawnCount = 0;
			for (TWeakObjectPtr<USpawnComponent> SpawnComponent : SpawnComponents)
			{
				if (USpawnComponent* SpawnComponentPtr = SpawnComponent.Get())
				{
					SpawnComponentPtr->Despawn();
					DespawnCount++;
				}
			}

			if (DespawnCount > 0)
			{
				TriggerOutput(TEXT("Despawned"), true);
			}
		}
	}
	else
	{
		LogError(MissingIdentityTag);
	}
}

bool UFlowNode_SpawnByActorReference::CanFastForward(const FName& InputPinName, const FName& OutputPinName) const
{
	return (InputPinName == TEXT("Despawn")) == (OutputPinName == TEXT("Despawned"));
}

void UFlowNode_SpawnByActorReference::Cleanup()
{
	SpawnComponents.Empty();

	Super::Cleanup();
}

#if WITH_EDITOR
FString UFlowNode_SpawnByActorReference::GetNodeDescription() const
{
	const FString ClassString = SpawnParams.ActorClass ? SpawnParams.ActorClass->GetFName().ToString() : TEXT("Missing Actor Class!");
	return (SpawnPoints.Num() > 0 ? FString::Printf(TEXT("%d spawn points"), SpawnPoints.Num()) : TEXT("No spawn points!")) + LINE_TERMINATOR + ClassString;
}

EDataValidationResult UFlowNode_SpawnByActorReference::ValidateNode()
{
	if (SpawnPoints.Num() == 0 || SpawnParams.ActorClass == nullptr)
	{
		if (SpawnPoints.Num() == 0)
		{
			ValidationLog.Error<UFlowNode>(TEXT("No Spawn Points assigned"), this);
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
