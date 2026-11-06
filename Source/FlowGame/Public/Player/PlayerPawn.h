// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Gameplay/MoverPawn.h"
#include "PlayerPawn.generated.h"

class UCameraComponent;
class UInputMappingContext;

class UTaggedInputComponent;

UCLASS()
class APlayerPawn : public AMoverPawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

private:
	TWeakObjectPtr<UTaggedInputComponent> TaggedInputComponent;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	void SetExplorationContext();
	void RemoveExplorationContext() const;

protected:
	virtual FVector GetDirectionalIntent() override;
};
