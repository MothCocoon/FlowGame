// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Gameplay/InteractionComponent.h"

#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractionComponent)

FPlayerInInteractionEvent UInteractionComponent::OnPlayerEnter;
FPlayerInInteractionEvent UInteractionComponent::OnPlayerExit;

UInteractionComponent::UInteractionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoActivate = true;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	SetUsingAbsoluteScale(true);
	ArrowColor = FColor::Red;
	ArrowSize = 0.5f;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bEnabled)
	{
		Enable();
	}
}

void UInteractionComponent::Enable()
{
	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		bEnabled = true;

		CameraManager = PlayerController->PlayerCameraManager;
		PrimaryComponentTick.SetTickFunctionEnable(true);
	}
}

void UInteractionComponent::Disable()
{
	if (bCanInteract)
	{
		bCanInteract = false;
		OnPlayerExit.Broadcast(this);
	}

	bEnabled = false;

	PrimaryComponentTick.SetTickFunctionEnable(false);
	CameraManager = nullptr;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool bConditionsMet = false;
	if (CameraManager.IsValid())
	{
		const FVector DistanceToCamera = GetComponentLocation() - CameraManager->GetCameraLocation();
		bConditionsMet = DistanceToCamera.Size() < Distance;
	}

	if (bConditionsMet)
	{
		if (!bCanInteract)
		{
			bCanInteract = true;
			OnPlayerEnter.Broadcast(this);
		}
	}
	else if (bCanInteract)
	{
		bCanInteract = false;
		OnPlayerExit.Broadcast(this);
	}
}
