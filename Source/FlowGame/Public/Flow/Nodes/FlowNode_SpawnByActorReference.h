// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Nodes/FlowNode.h"

#include "Gameplay/SpawnComponent.h"
#include "FlowNode_SpawnByActorReference.generated.h"

/**
 * Spawn by Actor Reference
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Spawn by Actor Reference"))
class UFlowNode_SpawnByActorReference : public UFlowNode
{
	GENERATED_BODY()
	
public:
	UFlowNode_SpawnByActorReference();

protected:
	UPROPERTY(EditAnywhere, Category = "SpawnPoint")
	TArray<TSoftObjectPtr<AActor>> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ShowOnlyInnerProperties))
	FQuestSpawnParams SpawnParams;

	TSet<TWeakObjectPtr<USpawnComponent>> SpawnComponents;
	
public:
	virtual void ExecuteInput(const FName& PinName) override;
	virtual bool CanFastForward(const FName& InputPinName, const FName& OutputPinName) const override;
	virtual void Cleanup() override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;

protected:
	virtual EDataValidationResult ValidateNode() override;
#endif
};
