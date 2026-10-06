// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors

#include "Gameplay/MoverPawn.h"

#include "FlowComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefaultMovementSet/NavMoverComponent.h"
#include "GameFramework/Controller.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavMesh/RecastNavMesh.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MoverPawn)

AMoverPawn::AMoverPawn()
	: RadiusOfReachablePoint(50.0f)
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(90.0f);
	CapsuleComponent->SetCapsuleRadius(30.0f);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SetRootComponent(CapsuleComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	MoverComponent = CreateDefaultSubobject<UMoverComponent>(TEXT("MoverComponent"));

	NavMoverComponent = CreateDefaultSubobject<UNavMoverComponent>(TEXT("NavMoverComponent"));
	NavMoverComponent->NavAgentProps.AgentRadius = 35.0;
	NavMoverComponent->NavAgentProps.AgentHeight = 180.0;
	NavMoverComponent->NavAgentProps.AgentStepHeight = 40.0;
	NavMoverComponent->NavAgentProps.PreferredNavData = ARecastNavMesh::StaticClass();
	NavMoverComponent->NavAgentProps.bCanWalk = true;

	SetReplicatingMovement(false); // disable Actor-level movement replication, Mover component will handle it

	FlowComponent = CreateDefaultSubobject<UFlowComponent>(TEXT("FlowComponent"));
}

FVector AMoverPawn::GetNavAgentLocation() const
{
	FVector AgentLocation = NavMoverComponent->GetFeetLocation();

	if (AgentLocation == FNavigationSystem::InvalidLocation)
	{
		AgentLocation = CapsuleComponent->GetComponentLocation() - FVector(0, 0, CapsuleComponent->Bounds.BoxExtent.Z);
	}

	return AgentLocation;
}

FGameplayTagContainer AMoverPawn::GetIdentityTags() const
{
	return FlowComponent->IdentityTags;
}

void AMoverPawn::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	FCharacterDefaultInputs& PawnInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

	// don't do anything if there's no local controller
	// simulated proxies will just use previous input when extrapolating
	if (Controller == nullptr)
	{
		if (GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
		{
			// If we get here, that means this pawn is not currently possessed, and we're choosing to provide default do-nothing input
			static const FCharacterDefaultInputs DoNothingInput;
			PawnInputs = DoNothingInput;
		}

		return;
	}

	PawnInputs.SetMoveInput(EMoveInputType::DirectionalIntent, GetDirectionalIntent());
	PawnInputs.ControlRotation = Controller->GetControlRotation();
}

FVector AMoverPawn::GetDirectionalIntent()
{
	// consume nav movement
	if (Controller && NavMoverComponent)
	{
		FVector MoveInputIntent;
		FVector MoveInputVelocity;
		NavMoverComponent->ConsumeNavMovementData(MoveInputIntent, MoveInputVelocity);

		return MoveInputIntent;
	}

	return FVector::ZeroVector;
}

void AMoverPawn::MoveToLocation(const FVector& Origin) const
{
	MoverComponent->QueueNextMode(TEXT("NavWalking"));

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		if (ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate))
		{
			// obtain target location
			FNavLocation ResultLocation;
			NavSys->GetRandomReachablePointInRadius(Origin, RadiusOfReachablePoint, ResultLocation, NavData, UNavigationQueryFilter::GetQueryFilter(*NavData, this, nullptr));

			// send to location
			UPathFollowingComponent* PathFollowingComponent = Controller->FindComponentByClass<UPathFollowingComponent>();
			if (PathFollowingComponent && PathFollowingComponent->IsPathFollowingAllowed())
			{
				if (PathFollowingComponent->HasReached(ResultLocation.Location, EPathFollowingReachMode::OverlapAgent))
				{
					// target location already reached
					PathFollowingComponent->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
				}
				else
				{
					// find path and move to location
					const FVector AgentNavLocation = Controller->GetNavAgentLocation();
					if (const ANavigationData* AgentNavData = NavSys->GetNavDataForProps(Controller->GetNavAgentPropertiesRef(), AgentNavLocation))
					{
						const FPathFindingQuery Query(Controller, *AgentNavData, AgentNavLocation, ResultLocation.Location);
						const FPathFindingResult Result = NavSys->FindPathSync(Query);

						if (Result.IsSuccessful())
						{
							PathFollowingComponent->OnRequestFinished.AddUObject(this, &ThisClass::OnMoveRequestFinished);
							PathFollowingComponent->RequestMove(FAIMoveRequest(ResultLocation.Location), Result.Path);
						}
						else if (PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
						{
							PathFollowingComponent->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
						}
					}
				}
			}
		}
	}
}

void AMoverPawn::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result) const
{
	if (UPathFollowingComponent* PathFollowingComponent = Controller->FindComponentByClass<UPathFollowingComponent>())
	{
		PathFollowingComponent->OnRequestFinished.RemoveAll(this);
	}

	OnMoveToLocationFinished.Broadcast(this, Result);
}
