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
			// ĳ���� �ӵ� ����
			PlayerCharacter->SetSpeedbyPercent(SlowPercentage);

			// ���� �ð� �� �ӵ� ����
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