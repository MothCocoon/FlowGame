// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "TaggedInputAction.h"
#include "TaggedInputComponent.generated.h"

/**
 * Allows to bind Input Actions without referencing Input Action asset.
 * Only Gameplay Tag needs to be provided, since we get iterate mappings listed in the Input Mapping Context
 */
UCLASS()
class UTaggedInputComponent : public UEnhancedInputComponent
{
	GENERATED_UCLASS_BODY()

protected:
	TMultiMap<FGameplayTag, uint32> HandlesByTag;
	TMap<FGameplayTag, TWeakObjectPtr<const UTaggedInputAction>> TaggedActionValues;

	virtual const UTaggedInputAction* GetInputAction(const UInputMappingContext* InputContext, const FGameplayTag& ActionTag);

public:
	template <class UserClass, typename FuncType>
	void BindActionByTag(const UInputMappingContext* InputContext, const FGameplayTag& ActionTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
	{
		if (const UTaggedInputAction* InputAction = GetInputAction(InputContext, ActionTag))
		{
			const FEnhancedInputActionEventBinding& Binding = BindAction(InputAction, TriggerEvent, Object, Func);
			HandlesByTag.Add(ActionTag, Binding.GetHandle());
		}
	}

	void BindValueByTag(const UInputMappingContext* InputContext, const FGameplayTag& ActionTag);
	FInputActionValue GetValueByTag(const FGameplayTag& ActionTag);

public:
	void RemoveBindingsByTag(const FGameplayTag& InputTag);
	void RemoveAllBindings();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
