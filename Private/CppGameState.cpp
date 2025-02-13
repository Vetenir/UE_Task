#include "CppGameState.h"
#include "CppGameInstance.h"
#include "CppPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ACppGameState::ACppGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 60.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWave = 0;
	MaxWave = 3;
	WaveDuration = 20.0f;
}

void ACppGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACppGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ACppGameState::GetScore() const
{
	return Score;
}

void ACppGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCppGameInstance* CppGameInstance = Cast<UCppGameInstance>(GameInstance);
		if (CppGameInstance)
		{
			CppGameInstance->AddToScore(Amount);
		}
	}
}

void ACppGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACppPlayerController* CppPlayerController = Cast<ACppPlayerController>(PlayerController))
		{
			CppPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCppGameInstance* CppGameInstance = Cast<UCppGameInstance>(GameInstance);
		if (CppGameInstance)
		{
			CurrentLevelIndex = CppGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	StartWave();

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ACppGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ACppGameState::OnWaveTimeUp,
		WaveDuration,
		true
	);

}

void ACppGameState::StartWave()
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 20;  // 웨이브마다 20개씩 스폰

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Wave %d Start!"), CurrentWave + 1));
}

void ACppGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ACppGameState::OnWaveTimeUp()
{
	CurrentWave++;

	if (CurrentWave < MaxWave)  // 3번째 웨이브(인덱스 2)까지 실행
	{
		StartWave();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(WaveTimerHandle); // 3웨이브 이후 타이머 정지
	}
}

void ACppGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount
	)

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}

}

void ACppGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCppGameInstance* CppGameInstance = Cast<UCppGameInstance>(GameInstance);
		if (CppGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			CppGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}

		if (CurrentLevelIndex >= MaxLevels)
		{
			OnGameOver();
			return;
		}
		if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
		{
			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
		}
		else
		{
			OnGameOver();
		}
	}
}

void ACppGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACppPlayerController* CppPlayerController = Cast<ACppPlayerController>(PlayerController))
		{
			CppPlayerController->SetPause(true);
			CppPlayerController->ShowMainMenu(true);
		}
	}
}

void ACppGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACppPlayerController* CppPlayerController = Cast<ACppPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = CppPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCppGameInstance* CppGameInstance = Cast<UCppGameInstance>(GameInstance);
						if (CppGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), CppGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}