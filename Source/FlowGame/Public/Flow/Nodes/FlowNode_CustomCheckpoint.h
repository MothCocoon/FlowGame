// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Nodes/FlowNode.h"
#include "FlowNode_CustomCheckpoint.generated.h"

/**
 *
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Checkpoint"))
class UFlowNode_CustomCheckpoint final : public UFlowNode
{
	GENERATED_BODY()
	
public:
	UFlowNode_CustomCheckpoint();

public:
	virtual void ExecuteInput(const FName& PinName) override;

protected:
	virtual void OnLoad_Implementation() override;
};
