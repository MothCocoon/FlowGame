// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/Nodes/FlowNode_OnInteractionUsed.h"

#include "Gameplay/InteractionComponent.h"

#include "FlowComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNode_OnInteractionUsed)

void UFlowNode_OnInteractionUsed::ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (!ObservedInteractions.Contains(Actor))
	{
		TArray<UInteractionComponent*> FoundInteractions;
		Actor->GetComponents<UInteractionComponent>(FoundInteractions);
		
		if (FoundInteractions.Num() > 0)
		{
			RegisteredActors.Emplace(Actor, Component);
			
			ObservedInteractions.Emplace(Actor, FoundInteractions[0]);
			FoundInteractions[0]->OnUsed.AddDynamic(this, &UFlowNode_OnInteractionUsed::OnEventReceived);
		}
	}
}

void UFlowNode_OnInteractionUsed::ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	ensureAlways(ObservedInteractions.Contains(Component->GetOwner()));
	const TWeakObjectPtr<UInteractionComponent> InteractionComponent = ObservedInteractions[Component->GetOwner()];
	
	InteractionComponent->OnUsed.RemoveAll(this);
	ObservedInteractions.Remove(Component->GetOwner());
}

void UFlowNode_OnInteractionUsed::FastForwardEvent()
{
	UInteractionComponent* UsedInteraction = nullptr;
	for (const TPair<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>>& Interaction : ObservedInteractions)
	{
		UInteractionComponent* ObservedInteraction = Interaction.Value.Get();
		if (UsedInteraction == nullptr && ObservedInteraction && ObservedInteraction->IsEnabled())
		{
			UsedInteraction = ObservedInteraction;
		}
	}

	// Player uses the interaction, so every listener of the interaction reacts
	if (UsedInteraction)
	{
		UsedInteraction->OnUsed.Broadcast();
	}
}

void UFlowNode_OnInteractionUsed::Cleanup()
{
	Super::Cleanup();

	for (const TPair<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>>& Interaction : ObservedInteractions)
	{
		Interaction.Value->OnUsed.RemoveAll(this);
	}
	ObservedInteractions.Empty();
}
