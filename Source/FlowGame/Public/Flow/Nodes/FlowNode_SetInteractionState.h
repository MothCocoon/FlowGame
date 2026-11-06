// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "GameplayTagContainer.h"

#include "Nodes/FlowNode.h"
#include "FlowNode_SetInteractionState.generated.h"

/**
 * Set Interaction State
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Set Interaction State"))
class UFlowNode_SetInteractionState : public UFlowNode
{
	GENERATED_BODY()
	
public:	
	UFlowNode_SetInteractionState();

protected:
	UPROPERTY(EditAnywhere, Category = "ObservedComponent")
	FGameplayTagContainer IdentityTags;
	
public:
	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;

protected:
	virtual EDataValidationResult ValidateNode() override;
#endif
};
