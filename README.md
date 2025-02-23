# [8번 과제] 게임 루프 및 UI 재설계하기

## 📌 프로젝트 개요
과거 프로젝트에서 구현된 아이템 인터페이스, 충돌 이벤트(Overlap), 런다운 스폰, 체력/점수, 웨이브/시간 제어, UI 등을 활용하여 게임 플레이와 UI/UX를 강화하는 작업입니다.

## 🎮 주요 기능
- 멀티 웨이브 구성: Wave 1 → Wave 2 → Wave 3 설정, 각 웨이브마다 제한 시간 적용
- 아이템 인터페이스: 각 웨이브에 맞는 아이템 스폰 개수 다르게 설정
- UI 재설계: 점수, 시간, 체력 등의 정보를 직관적으로 표시
- 메인 메뉴 및 게임 오버 메뉴 재설계

## 🛠 구현 내용

### ✅ 1. 멀티 웨이브 구성
- SpawnVolume, 충돌(Overlap), GameState/GameMode 등을 제어하지 않고 다시 활용
- 각 웨이브마다 제한 시간 적용
- 웨이브 진행 시 아이템 스폰 개수 조정

### ✅ 2. UI 분석 및 요구사항 정리
- HUD에 표시할 정보 정리 (점수, 시간, 체력 등)
- 메인 메뉴/종료 메뉴 등 UI 요구사항 분석

### ✅ 3. HUD & 메뉴 UI 재설계
- HUD 재디자인: CanvasPanel, VerticalBox 적용
- 플레이어 획득 점수, 시간, 체력 표시
- 메뉴 UI 재디자인: 시작, 종료 버튼 및 각종 방향 시작
- 확장 디자인: Hover, Clicked 효과 등 구현

## 🚀 도전 과제
- **아이템 효과 추가**: 이동 속도 감소, 캐릭터 획득 및 반전 등
- **웨이브 복잡화**: 맵 화면 변화, 무장위 반전 등
- **디자인 효과 구현**: 휴가, 테보 UI 등

## 📺 시연 영상
[YouTube 시연 영상](https://youtu.be/pz4Um6RP70g?si=_Y16qq6E_LEUxY7d)
