// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Nodes/Actor/FlowNode_ComponentObserver.h"
#include "FlowNode_OnInteractionUsed.generated.h"

class UFlowComponent;
class UInteractionComponent;

/**
 * On Interaction Used
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "On Interaction Used"))
class  UFlowNode_OnInteractionUsed : public UFlowNode_ComponentObserver
{
	GENERATED_BODY()
		
protected:
	TMap<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>> ObservedInteractions;

protected:
	virtual void ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;
	virtual void ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;

	virtual void FastForwardEvent() override;

public:
	virtual void Cleanup() override;
};
