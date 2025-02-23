#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	DebuffWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DebuffWidget"));
	DebuffWidget->SetupAttachment(GetMesh());
	DebuffWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 2.0f;
	SprintSpeed = NormalSpeed*SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100.0f;
	Health = MaxHealth;

	bIsSlowed = false;
	bIsReversed = false;
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move
				);
			}
				

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint
				);
			}
		}
	}
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
	UpdateDebuff();
}


void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();

	float DirectionMultiplier = bIsReversed ? -1.0f : 1.0f;

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X * DirectionMultiplier);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y * DirectionMultiplier);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float ASpartaCharacter::GetHealth() const
{
	return Health;
}

// ü�� ȸ�� �Լ�
void ASpartaCharacter::AddHealth(float Amount)
{
	// ü���� ȸ����Ŵ. �ִ� ü���� �ʰ����� �ʵ��� ������
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

// �ӵ� set �Լ�
void ASpartaCharacter::SetSpeedbyPercent(float Amount)
{
	// ���ǵ� ����
	NormalSpeed = NormalSpeed*Amount;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	bIsSlowed = true;
	UpdateDebuff();
}

void ASpartaCharacter::ResetSpeed()
{
	// ���ǵ� ����
	NormalSpeed = 600.0f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	bIsSlowed = false;
	UpdateDebuff();
}

// ������ ó�� �Լ�
float ASpartaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// �⺻ ������ ó�� ���� ȣ�� (�ʼ��� �ƴ�)
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// ü���� ��������ŭ ���ҽ�Ű��, 0 ���Ϸ� �������� �ʵ��� Clamp
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	// ü���� 0 ���ϰ� �Ǹ� ��� ó��
	if (Health <= 0.0f)
	{
		OnDeath();
	}

	// ���� ����� �������� ��ȯ
	return ActualDamage;
}

// ��� ó�� �Լ�
void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}

void ASpartaCharacter::ReverseControls(float Duration)
{
	bIsReversed = true;
	UpdateDebuff();
	// Restore controls after the duration
	GetWorld()->GetTimerManager().SetTimer(
		ReverseControlTimerHandle,
		this,
		&ASpartaCharacter::RestoreControls,
		Duration,
		false
	);
}

void ASpartaCharacter::RestoreControls()
{
	bIsReversed = false;
	UpdateDebuff();
}

void ASpartaCharacter::UpdateDebuff()
{
	if (!DebuffWidget) return;

	UUserWidget* DebuffWidgetInstance = DebuffWidget->GetUserWidgetObject();
	if (!DebuffWidgetInstance) return;

	// ReverseControl �̹��� ã��
	if (UImage* ReverseControlImage = Cast<UImage>(DebuffWidgetInstance->GetWidgetFromName(TEXT("ReverseControl"))))
	{
		ReverseControlImage->SetVisibility(bIsReversed ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	// Slow �̹��� ã�� (ĳ���� �ӵ��� �����ߴ��� Ȯ�� �ʿ�)
	if (UImage* SlowImage = Cast<UImage>(DebuffWidgetInstance->GetWidgetFromName(TEXT("Slow"))))
	{
		SlowImage->SetVisibility(bIsSlowed ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}