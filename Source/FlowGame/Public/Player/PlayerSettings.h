// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Engine/DeveloperSettings.h"
#include "PlayerSettings.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Player"))
class UPlayerSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Input")
	TSoftClassPtr<UUserWidget> InteractionWidget;

	// Contains debug inputs, inactive in Shipping builds
	UPROPERTY(Config, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UInputMappingContext> DebugContext;

	UPROPERTY(Config, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UInputMappingContext> ExplorationContext;
};
