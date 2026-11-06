// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "GameplayTagContainer.h"

#include "Nodes/FlowNode.h"

#include "Gameplay/SpawnComponent.h"
#include "FlowNode_SpawnByGameplayTag.generated.h"

/**
 * Spawn by Gameplay Tag
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Spawn by Gameplay Tag"))
class UFlowNode_SpawnByGameplayTag : public UFlowNode
{
	GENERATED_BODY()
	
public:
	UFlowNode_SpawnByGameplayTag();

protected:
	UPROPERTY(EditAnywhere, Category = "SpawnPoint")
	FGameplayTagContainer IdentityTags;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ShowOnlyInnerProperties))
	FQuestSpawnParams SpawnParams;

	TSet<TWeakObjectPtr<USpawnComponent>> SpawnComponents;
	
public:
	virtual void ExecuteInput(const FName& PinName) override;
	virtual bool CanFastForward(const FName& InputPinName, const FName& OutputPinName) const override;
	virtual void Cleanup() override;

protected:
	virtual void OnLoad_Implementation() override;

	void SpawnActors(const bool bKeepSpawnedActors);

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;

protected:
	virtual EDataValidationResult ValidateNode() override;
#endif
};
