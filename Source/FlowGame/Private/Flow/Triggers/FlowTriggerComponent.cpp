// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/Triggers/FlowTriggerComponent.h"

#include "FlowSubsystem.h"

#include "Components/BrushComponent.h"
#include "Components/ShapeComponent.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowTriggerComponent)

void UFlowTriggerComponent::NotifyFromGraph(const FGameplayTagContainer& NotifyTags, const EFlowNetMode NetMode /* = EFlowNetMode::Authority*/)
{
	if (NotifyTags.HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Flow.Common.Enable"))))
	{
		EnableOverlap();
	}
	else if (NotifyTags.HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Flow.Common.Disable"))))
	{
		DisableOverlap();
	}
	
	Super::NotifyFromGraph(NotifyTags, NetMode);
}

void UFlowTriggerComponent::EnableOverlap()
{
	if (!bOverlapEnabled)
	{
		bOverlapEnabled = true;

		for (UActorComponent* Component : GetOwner()->GetComponents())
		{
			if (Component && (Component->IsA(UShapeComponent::StaticClass()) || Component->IsA(UBrushComponent::StaticClass())))
			{
				UPrimitiveComponent* CollisionComponent = Cast<UPrimitiveComponent>(Component);
				if (CollisionComponent->GetGenerateOverlapEvents() && CollisionComponent->GetCollisionProfileName() == TEXT("Trigger"))
				{
					CollisionComponents.Emplace(CollisionComponent);

					CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &UFlowTriggerComponent::OnComponentBeginOverlap);
					CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &UFlowTriggerComponent::OnComponentEndOverlap);
				}
			}
		}
	}
}

void UFlowTriggerComponent::DisableOverlap()
{
	if (bOverlapEnabled)
	{
		bOverlapEnabled = false;

		for (TWeakObjectPtr<UPrimitiveComponent>& CollisionComponent : CollisionComponents)
		{
			if (CollisionComponent.IsValid())
			{
				CollisionComponent->OnComponentBeginOverlap.RemoveAll(this);
				CollisionComponent->OnComponentEndOverlap.RemoveAll(this);
			}
		}
		CollisionComponents.Empty();
	}
}

void UFlowTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoEnable)
	{
		EnableOverlap();
	}
}

void UFlowTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DisableOverlap();

	Super::EndPlay(EndPlayReason);
}

void UFlowTriggerComponent::OnComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	TArray<UFlowComponent*> FlowComponents;
	InOtherActor->GetComponents<UFlowComponent>(FlowComponents);

	if (FlowComponents.Num() > 0)
	{
		if (GetOwner()->HasActorBegunPlay())
		{
			OnTriggerEvent.Broadcast(true, FlowComponents[0]);
		}
		else
		{
			TWeakObjectPtr<UFlowTriggerComponent> SelfWeakPtr(this);
			TWeakObjectPtr<UFlowComponent> FlowComponentWeakPtr(FlowComponents[0]);
			GetOwner()->GetWorldTimerManager().SetTimerForNextTick([SelfWeakPtr, FlowComponentWeakPtr]()
			{
				if (SelfWeakPtr.IsValid())
				{
					SelfWeakPtr->OnTriggerEvent.Broadcast(true, FlowComponentWeakPtr.Get());
				}
			});
		}
	}
}

void UFlowTriggerComponent::OnComponentEndOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex)
{
	TArray<UFlowComponent*> FlowComponents;
	InOtherActor->GetComponents<UFlowComponent>(FlowComponents);

	if (FlowComponents.Num() > 0)
	{
		OnTriggerEvent.Broadcast(false, FlowComponents[0]);
	}
}
