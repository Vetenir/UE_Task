# 언리얼CPP
Cpp코드로 아래의 기능을 구현.



### Moving Block
* Tick()을 활용해서 액터의 왕복 이동 구현.

## Rotation Block
* Tick()을 활용해서 액터의 회전 구현.

## CppPawn
* 캐릭터의 이동과 카메라 회전 구현.

## CppGameState
* 게임의 전체적인 상태
  + 과제로서 코드에 추가한 것은 각 레벨 내의 Wave를 3단계로 구현.
  + 플레이어가 다음 Wave 이전에 빠르게 동전을 다 획득하여 다음 레벨로 넘어가거나
  + 폭탄도 추가적으로 생성되지만 더 높은 점수를 위해 남은 Wave를 기다리는 선택을 할 수 있음.
  + Wave 알림은 AddOnScreenDebugMessage로 구현.

## CppPlayerController
* 캐릭터의 이동과 카메라 회전, UI 구현.
  + 과제로서 코드에 추가한 것은 최종 화면에 최종 체력도 함께 출력 구현.
  + 블루프린트로 각 버튼에 맞는 이벤트 적용
    
![Image](https://github.com/user-attachments/assets/f08199f5-63cf-431d-8f6c-bf43be6be0d0)
