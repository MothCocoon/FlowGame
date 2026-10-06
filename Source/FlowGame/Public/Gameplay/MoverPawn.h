// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "GameFramework/Pawn.h"
#include "MoverSimulationTypes.h"
#include "MoverPawn.generated.h"

class UCapsuleComponent;
class UMoverComponent;
class UNavMoverComponent;
struct FPathFollowingResult;
struct FAIRequestID;

class UFlowComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPathFollowingEvent, const class AMoverPawn* /*Pawn*/, const FPathFollowingResult& /*Result*/)

UCLASS()
class AMoverPawn : public APawn, public IMoverInputProducerInterface
{
	GENERATED_BODY()

public:
	AMoverPawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<UMoverComponent> MoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<UNavMoverComponent> NavMoverComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<UFlowComponent> FlowComponent;

public:
	UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
	USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent; }
	UMoverComponent* GetMoverComponent() const { return MoverComponent; }
	UNavMoverComponent* GetNavMoverComponent() const { return NavMoverComponent; }
	UFlowComponent* GetFlowComponent() const { return FlowComponent; }

	// INavAgentInterface
	virtual FVector GetNavAgentLocation() const override;
	
	UFUNCTION(BlueprintPure, Category = "MossPawn")
	FGameplayTagContainer GetIdentityTags() const;

protected:
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;
	virtual FVector GetDirectionalIntent();

protected:
	// Used when obtaining reachable point around MoveTo target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MossMover")
	float RadiusOfReachablePoint;

public:
	FOnPathFollowingEvent OnMoveToLocationFinished;
	
	UFUNCTION(BlueprintCallable, Category = "MossMover")
	void MoveToLocation(const FVector& Origin) const;

protected:
	void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result) const;
};
