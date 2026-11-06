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
	if (TaggedInputComponent.IsValid())
	{
		const UInputMappingContext* ExplorationContext = GetDefault<UPlayerSettings>()->ExplorationContext.LoadSynchronous();

		TaggedInputComponent->BindValueByTag(ExplorationContext, PlayerTags::Input_MoveForward);
		TaggedInputComponent->BindValueByTag(ExplorationContext, PlayerTags::Input_MoveRight);
	}
}

void APlayerPawn::RemoveExplorationContext() const
{
	if (TaggedInputComponent.IsValid())
	{
		TaggedInputComponent->RemoveBindingsByTag(PlayerTags::Input_MoveForward);
		TaggedInputComponent->RemoveBindingsByTag(PlayerTags::Input_MoveRight);
	}
}

FVector APlayerPawn::GetDirectionalIntent()
{
	FVector Result;

	// read movement direction from player input
	if (TaggedInputComponent.IsValid())
	{
		Result.X = TaggedInputComponent->GetValueByTag(PlayerTags::Input_MoveForward).Get<float>();
		Result.Y = TaggedInputComponent->GetValueByTag(PlayerTags::Input_MoveRight).Get<float>();
	}

	return Result;
}
