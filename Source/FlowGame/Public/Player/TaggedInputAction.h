// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "TaggedInputAction.generated.h"

/**
 * 
 */
UCLASS()
class UTaggedInputAction : public UInputAction
{
	GENERATED_BODY()

public:
	// Native actor will create input binding by this tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tag", meta = (Categories = "Input"))
	FGameplayTag ActionTag;
};
