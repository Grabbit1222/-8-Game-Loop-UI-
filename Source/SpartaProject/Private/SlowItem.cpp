#include "SlowItem.h"
#include "SpartaCharacter.h"

ASlowItem::ASlowItem()
{
	SlowPercentage = 0.5f;
	SlowTime = 5.0f;
	ItemType = "Debuff";
}

void ASlowItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			// 캐릭터 속도 감소
			PlayerCharacter->SetSpeedbyPercent(SlowPercentage);

			// 일정 시간 후 속도 복구
			GetWorld()->GetTimerManager().SetTimer(
				SlowTimerHandle,
				FTimerDelegate::CreateLambda([PlayerCharacter]() {
					if (PlayerCharacter)
					{
						PlayerCharacter->ResetSpeed();
					}
					}),
				SlowTime,
				false
			);
		}

		DestroyItem();
	}
}