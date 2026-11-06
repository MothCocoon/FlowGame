// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "FlowComponent.h"
#include "FlowTriggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFlowTriggerComponentEvent, const bool, bOverlapping, class UFlowComponent*, OtherFlowComponent);

/**
* Trigger-specific Flow component - encapsulates communication between triggers and Flow graph
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class UFlowTriggerComponent final : public UFlowComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FlowTrigger")
	bool bAutoEnable = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "FlowTrigger")
	bool bOverlapEnabled = false;

	UPROPERTY(BlueprintAssignable, Category = "FlowTrigger")
	FFlowTriggerComponentEvent OnTriggerEvent;
	
	TArray<TWeakObjectPtr<UPrimitiveComponent>> CollisionComponents;

public:
	virtual void NotifyFromGraph(const FGameplayTagContainer& NotifyTags, const EFlowNetMode NetMode) override;

	void EnableOverlap();
	void DisableOverlap();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult);
	
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex);
};
