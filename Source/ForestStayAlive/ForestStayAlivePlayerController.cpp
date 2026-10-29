// Fill out your copyright notice in the Description page of Project Settings.


#include "ForestStayAlivePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "UI/CameraWidget.h"
#include "UI/MainUIWidget.h"
#include "EnhancedInputComponent.h"
#include "ForestStayAliveCharacter.h"
#include "InputAction.h"
#include "UI/Components/PlayerCrosshairWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

void AForestStayAlivePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerCrosshairWidgetClass)
	{
		PlayerCrosshairWidget = CreateWidget<UPlayerCrosshairWidget>(
			this,
			PlayerCrosshairWidgetClass
		);
		
		if (PlayerCrosshairWidget)
		{
			PlayerCrosshairWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
			PlayerCrosshairWidget->AddToViewport(1000);
		}
	}
	
	if (MainUIWidgetClass)
	{
		MainUIWidget = CreateWidget<UMainUIWidget>(
			this,
			MainUIWidgetClass
		);

		if (MainUIWidget)
		{
			MainUIWidget->PlayerController = this;
			MainUIWidget->AddToViewport();
		}
	}
	
	if (CameraWidgetClass)
	{
		CameraWidget = CreateWidget<UCameraWidget>(
			this,
			CameraWidgetClass
		);
	}
	
	
	
}

void AForestStayAlivePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInput) return;

	if (RightClickAction)
	{
		EnhancedInput->BindAction(
			RightClickAction,
			ETriggerEvent::Started,
			this,
			&AForestStayAlivePlayerController::OnRightClickStarted
		);
	}
	
	if (LeftClickAction)
	{
		EnhancedInput->BindAction(
			LeftClickAction,
			ETriggerEvent::Triggered,
			this,
			&AForestStayAlivePlayerController::OnLeftClickHold
		);
		
		EnhancedInput->BindAction(
			LeftClickAction,
			ETriggerEvent::Completed,
			this,
			&AForestStayAlivePlayerController::OnLeftClickCompleted
		);
	}
}

void AForestStayAlivePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
	if (PlayerCrosshairWidget->InteractProgress != 1.0f)
	{
		int32 ViewportWidth = 0;
		int32 ViewportHeight = 0;
		GetViewportSize(ViewportWidth, ViewportHeight);

		SetMouseLocation(
			ViewportWidth / 2,
			ViewportHeight / 2
		);
	}
	else
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
	}
	
	
	if (PlayerCrosshairWidget)
	{
		float MouseX;
		float MouseY;
		
		if (GetMousePosition(MouseX, MouseY))
		{
			PlayerCrosshairWidget->SetPositionInViewport(
				FVector2D(MouseX, MouseY),
				true
			);
		}
	}
}

void AForestStayAlivePlayerController::OnRightClickStarted(const FInputActionValue& Value)
{
	if (!CameraWidget) return;
	
	if (CameraWidget->IsInViewport())
	{
		CameraWidget->RemoveFromViewport();
		MainUIWidget->AddToViewport();
		if (ForestStayAliveCharacter)
		{
			ForestStayAliveCharacter->SetCameraFOV(70.0f);
			ForestStayAliveCharacter->SetTargetFOV(70.0f);
		}
	}
	else
	{
		MainUIWidget->RemoveFromViewport();
		CameraWidget->AddToViewport();
	}
}

void AForestStayAlivePlayerController::OnLeftClickHold(const FInputActionValue& Value)
{
	if (PlayerCrosshairWidget)
	{
		PlayerCrosshairWidget->SetCrosshairType(FText::FromString(TEXT("Interacting")));
	}
	
	PlayerCrosshairWidget->bIsInteracting = true;
}

void AForestStayAlivePlayerController::OnLeftClickCompleted(const FInputActionValue& Value)
{
	if (PlayerCrosshairWidget)
	{
		PlayerCrosshairWidget->SetCrosshairType(FText::FromString(TEXT("Interactable")));
	}
	
	SetInputMode(FInputModeGameOnly());
	
	
	PlayerCrosshairWidget->bIsInteracting = false;
	PlayerCrosshairWidget->InteractProgress = 0.0f;
}
