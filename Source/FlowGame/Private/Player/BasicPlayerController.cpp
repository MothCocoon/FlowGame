// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors

#include "Player/BasicPlayerController.h"

#include "Gameplay/InteractionComponent.h"
#include "Player/BasicUIManager.h"
#include "Player/PlayerPawn.h"
#include "Player/PlayerSettings.h"
#include "Player/PlayerTags.h"
#include "Player/TaggedInputComponent.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasicPlayerController)

ABasicPlayerController::ABasicPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	UInteractionComponent::OnPlayerEnter.AddUObject(this, &ABasicPlayerController::OnInteractionEnter);
	UInteractionComponent::OnPlayerExit.AddUObject(this, &ABasicPlayerController::OnInteractionExit);
}

void ABasicPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	TaggedInputComponent = Cast<UTaggedInputComponent>(InputComponent);
}

void ABasicPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetExplorationContext();
}

void ABasicPlayerController::SetExplorationContext()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();

		const UPlayerSettings* PlayerSettings = GetDefault<UPlayerSettings>();

#if !UE_BUILD_SHIPPING
		if (const UInputMappingContext* DebugContext = PlayerSettings->DebugContext.LoadSynchronous())
		{
			Subsystem->AddMappingContext(DebugContext, 0);
		}
#endif

		// Exploration context
		if (const UInputMappingContext* ExplorationContext = PlayerSettings->ExplorationContext.LoadSynchronous())
		{
			Subsystem->AddMappingContext(ExplorationContext, 1);

			// movement handled by Mover
			if (APlayerPawn* PlayerPawn = GetPawn<APlayerPawn>())
			{
				PlayerPawn->SetExplorationContext();
			}

			if (TaggedInputComponent.IsValid())
			{
				// camera
				TaggedInputComponent->BindValueByTag(ExplorationContext, PlayerTags::Input_CameraPitch);
				TaggedInputComponent->BindActionByTag(ExplorationContext, PlayerTags::Input_CameraPitch, ETriggerEvent::Triggered, this, &ThisClass::AddPitch);
				
				TaggedInputComponent->BindValueByTag(ExplorationContext, PlayerTags::Input_CameraYaw);
				TaggedInputComponent->BindActionByTag(ExplorationContext, PlayerTags::Input_CameraYaw, ETriggerEvent::Triggered, this, &ThisClass::AddYaw);

				// interaction
				TaggedInputComponent->BindActionByTag(ExplorationContext, PlayerTags::Input_Interaction, ETriggerEvent::Started, this, &ThisClass::OnInteractionUsed);
			}
		}
	}
}

void ABasicPlayerController::AddPitch()
{
	if (TaggedInputComponent.IsValid())
	{
		RotationInput.Pitch += TaggedInputComponent->GetValueByTag(PlayerTags::Input_CameraPitch).Get<float>();
	}
}

void ABasicPlayerController::AddYaw()
{
	if (TaggedInputComponent.IsValid())
	{
		RotationInput.Yaw += TaggedInputComponent->GetValueByTag(PlayerTags::Input_CameraYaw).Get<float>();
	}
}

void ABasicPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if (PossibleInteractions.Num() > 0)
	{
		const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		PossibleInteractions.Sort([CameraLocation](const TWeakObjectPtr<UInteractionComponent>& A, const TWeakObjectPtr<UInteractionComponent>& B)
		{
			const float DistanceA = FVector::DistSquared(CameraLocation, A.Get()->GetComponentLocation());
			const float DistanceB = FVector::DistSquared(CameraLocation, B.Get()->GetComponentLocation());
			return DistanceA < DistanceB;
		});

		if (!ActiveInteraction.IsValid() && ActiveInteraction != PossibleInteractions[0])
		{
			ActivateInteraction(PossibleInteractions[0]);
		}
	}
	else if (ActiveInteraction.IsValid())
	{
		DeactivateInteraction();
	}
}

void ABasicPlayerController::OnInteractionEnter(const TWeakObjectPtr<UInteractionComponent> Interaction)
{
	PossibleInteractions.Add(Interaction);
}

void ABasicPlayerController::OnInteractionExit(const TWeakObjectPtr<UInteractionComponent> Interaction)
{
	if (ActiveInteraction.IsValid() && ActiveInteraction == Interaction)
	{
		DeactivateInteraction();
	}

	PossibleInteractions.Remove(Interaction);
}

void ABasicPlayerController::ActivateInteraction(const TWeakObjectPtr<UInteractionComponent> Interaction)
{
	if (ActiveInteraction.IsValid())
	{
		DeactivateInteraction();
	}

	ActiveInteraction = Interaction;
	GetLocalPlayer()->GetSubsystem<UBasicUIManager>()->OpenWidget(GetDefault<UPlayerSettings>()->InteractionWidget);
}

void ABasicPlayerController::DeactivateInteraction()
{
	ActiveInteraction = nullptr;
	GetLocalPlayer()->GetSubsystem<UBasicUIManager>()->CloseWidget(GetDefault<UPlayerSettings>()->InteractionWidget);
}

void ABasicPlayerController::OnInteractionUsed()
{
	if (ActiveInteraction.IsValid())
	{
		ActiveInteraction->OnUsed.Broadcast();
	}
}
