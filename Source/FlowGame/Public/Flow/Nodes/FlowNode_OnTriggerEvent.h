// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Nodes/Actor/FlowNode_ComponentObserver.h"
#include "FlowNode_OnTriggerEvent.generated.h"

class UFlowComponent;

/**
 * On Trigger Event
 */
UCLASS(Abstract, NotBlueprintable)
class  UFlowNode_OnTriggerEvent : public UFlowNode_ComponentObserver
{
	GENERATED_BODY()
		
public:
	UFlowNode_OnTriggerEvent();

protected:
	UPROPERTY(EditAnywhere, Category = "Trigger")
	FGameplayTagContainer OverlappedActorTags;

	bool bReactOnOverlapping = false;

public:
	virtual void ExecuteInput(const FName& PinName) override;

protected:
	virtual void ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;
	virtual void ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;

	UFUNCTION()
	virtual void OnTriggerEvent(const bool bOverlapping, UFlowComponent* OtherFlowComponent);

	virtual void FastForwardEvent() override;

#if WITH_EDITOR
	virtual EDataValidationResult ValidateNode() override;
#endif
};
