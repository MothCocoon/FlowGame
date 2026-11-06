// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "FlowSubsystem.h"
#include "FlowSaveSubsystem.generated.h"

/**
 * Minimal logic, just enough for testing purposes
 * WORK IN PROGRESS
 */
UCLASS()
class UFlowSaveSubsystem final : public UFlowSubsystem, public FSelfRegisteringExec
{
	GENERATED_BODY()

public:
	static FString CheckpointSlotName;

	// FSelfRegisteringExec
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	void SaveGame();

	UFUNCTION(Exec, Category = "SaveSubsystem")
	void LoadGame();
};
