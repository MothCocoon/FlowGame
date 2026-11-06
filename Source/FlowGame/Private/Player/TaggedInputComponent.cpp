// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Player/TaggedInputComponent.h"

#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TaggedInputComponent)

UTaggedInputComponent::UTaggedInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(FObjectInitializer::Get())
{
}

const UTaggedInputAction* UTaggedInputComponent::GetInputAction(const UInputMappingContext* InputContext, const FGameplayTag& ActionTag)
{
	ensureAlwaysMsgf(InputContext, TEXT("TaggedInputComponent::GetInputAction called with invalid Input Mapping Context"));
	ensureAlwaysMsgf(ActionTag.IsValid(), TEXT("TaggedInputComponent::GetInputAction called with invalid Action Tag"));

	if (InputContext && ActionTag.IsValid())
	{
		for (const FEnhancedActionKeyMapping& Mapping : InputContext->GetMappings())
		{
			if (Mapping.Action)
			{
				const UTaggedInputAction* InputAction = Cast<UTaggedInputAction>(Mapping.Action);
				if (InputAction && InputAction->ActionTag.MatchesTagExact(ActionTag))
				{
					return InputAction;
				}
			}
		}
	}

	return nullptr;
}

void UTaggedInputComponent::BindValueByTag(const UInputMappingContext* InputContext, const FGameplayTag& ActionTag)
{
	if (const UTaggedInputAction* InputAction = GetInputAction(InputContext, ActionTag))
	{
		BindActionValue(InputAction);
		TaggedActionValues.Add(ActionTag, InputAction);
	}
}

FInputActionValue UTaggedInputComponent::GetValueByTag(const FGameplayTag& ActionTag)
{
	if (const TWeakObjectPtr<const UTaggedInputAction>* InputAction = TaggedActionValues.Find(ActionTag))
	{
		return GetBoundActionValue(InputAction->Get());
	}

	return FInputActionValue(EInputActionValueType::Axis3D, FVector::ZeroVector);
}

void UTaggedInputComponent::RemoveBindingsByTag(const FGameplayTag& InputTag)
{
	TArray<uint32> Handles;
	HandlesByTag.MultiFind(InputTag, Handles);
	for (const uint32& Handle : Handles)
	{
		RemoveBindingByHandle(Handle);
	}

	HandlesByTag.Remove(InputTag);
}

void UTaggedInputComponent::RemoveAllBindings()
{
	ClearActionBindings();
	TaggedActionValues.Empty();
	HandlesByTag.Empty();
}

void UTaggedInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllBindings();

	Super::EndPlay(EndPlayReason);
}

