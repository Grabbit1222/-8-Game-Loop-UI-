#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	//이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

}

void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		ActivateItem(OtherActor);
	}
}

void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

// 이펙트 생성 & 타이머 설정
void ABaseItem::ActivateItem(AActor* Activator)
{
    if (PickupNiagaraSystem)
    {
        NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            PickupNiagaraSystem,
            GetActorLocation(),
            GetActorRotation(),
            FVector(1.0f),
            false  // 직접 삭제
        );

        if (NiagaraComponent)
        {
            // 2초 후 DestroyNiagaraEffect() 함수 호출
            GetWorld()->GetTimerManager().SetTimer(
                DestroyNiagaraTimerHandle,
                this,
                &ABaseItem::DestroyNiagaraEffect,
                2.0f,  // 2초 후 실행
                false
            );
        }
    }

    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            PickupSound,
            GetActorLocation()
        );
    }
}

void ABaseItem::DestroyNiagaraEffect()
{
    if (NiagaraComponent)
    {
        NiagaraComponent->DestroyComponent();
        NiagaraComponent = nullptr;
    }
}


FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}