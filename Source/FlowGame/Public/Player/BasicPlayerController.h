// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "GameFramework/PlayerController.h"
#include "BasicPlayerController.generated.h"

class UInteractionComponent;

/**
 *
 */
UCLASS()
class ABasicPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

private:
	TWeakObjectPtr<class UTaggedInputComponent> TaggedInputComponent;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

public:
	void SetExplorationContext();
	void AddPitch();
	void AddYaw();

	virtual void PlayerTick(float DeltaTime) override;

private:
	TArray<TWeakObjectPtr<UInteractionComponent>> PossibleInteractions;
	TWeakObjectPtr<UInteractionComponent> ActiveInteraction;

	void OnInteractionEnter(const TWeakObjectPtr<UInteractionComponent> Interaction);
	void OnInteractionExit(const TWeakObjectPtr<UInteractionComponent> Interaction);

	void ActivateInteraction(const TWeakObjectPtr<UInteractionComponent> Interaction);
	void DeactivateInteraction();

	void OnInteractionUsed();
};
