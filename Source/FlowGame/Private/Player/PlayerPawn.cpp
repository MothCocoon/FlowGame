// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Player/PlayerPawn.h"

#include "Player/PlayerSettings.h"
#include "Player/PlayerTags.h"
#include "Player/TaggedInputComponent.h"

#include "FlowComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlayerPawn)

APlayerPawn::APlayerPawn()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	CameraComponent->bUsePawnControlRotation = true;

	GetFlowComponent()->IdentityTags = FGameplayTagContainer(PlayerTags::Player_Pawn);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	TaggedInputComponent = Cast<UTaggedInputComponent>(InputComponent);
}

void APlayerPawn::SetExplorationContext()
{
	if (UTaggedInputComponent* TaggedInputComponentPtr = TaggedInputComponent.Get())
	{
		const UInputMappingContext* ExplorationContext = GetDefault<UPlayerSettings>()->ExplorationContext.LoadSynchronous();

		TaggedInputComponentPtr->BindValueByTag(ExplorationContext, PlayerTags::Input_MoveForward);
		TaggedInputComponentPtr->BindValueByTag(ExplorationContext, PlayerTags::Input_MoveRight);
	}
}

void APlayerPawn::RemoveExplorationContext() const
{
	if (UTaggedInputComponent* TaggedInputComponentPtr = TaggedInputComponent.Get())
	{
		TaggedInputComponentPtr->RemoveBindingsByTag(PlayerTags::Input_MoveForward);
		TaggedInputComponentPtr->RemoveBindingsByTag(PlayerTags::Input_MoveRight);
	}
}

FVector APlayerPawn::GetDirectionalIntent()
{
	FVector Result;

	// read movement direction from player input
	if (UTaggedInputComponent* TaggedInputComponentPtr = TaggedInputComponent.Get())
	{
		Result.X = TaggedInputComponentPtr->GetValueByTag(PlayerTags::Input_MoveForward).Get<float>();
		Result.Y = TaggedInputComponentPtr->GetValueByTag(PlayerTags::Input_MoveRight).Get<float>();
	}

	return Result;
}
