#include "UI/MyTestUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"

UMyTestUserWidget::UMyTestUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PbHpBar = nullptr;
}

void UMyTestUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 디버깅을 위한 로그 추가
    UE_LOG(LogTemp, Log, TEXT("NativeConstruct Called"));

    if (!PbHpBar)
    {
        UE_LOG(LogTemp, Log, TEXT("Creating HpBar"));
        CreateHpBar();
    }

    InitializeHpBar();

    if (!ensure(PbHpBar))
    {
        UE_LOG(LogTemp, Error, TEXT("PbHpBar is still null after creation attempt"));
    }

    if (PbHpBar)
    {
        UE_LOG(LogTemp, Error, TEXT("PbHpBar is exist"));
    }

    if (!GetRootWidget())
    {
        UE_LOG(LogTemp, Error, TEXT("Root widget is not set!"));
    }

    if (GetRootWidget())
    {
        UE_LOG(LogTemp, Error, TEXT("Root widget is exist"));
    }
}

void UMyTestUserWidget::InitializeHpBar()
{
    if (PbHpBar)
    {
        // 프로그레스바 색상을 파란색으로 설정
        PbHpBar->SetFillColorAndOpacity(FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));

        // 프로그레스바를 50%로 설정
        PbHpBar->SetPercent(0.5f);
    }
}

void UMyTestUserWidget::CreateHpBar()
{
    // 1. 먼저 VerticalBox가 있는지 확인하고 없으면 생성
    UVerticalBox* RootVerticalBox = Cast<UVerticalBox>(GetRootWidget());
    if (!RootVerticalBox)
    {
        // Root VerticalBox 생성
        RootVerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootVerticalBox"));
        WidgetTree->RootWidget = RootVerticalBox; // 루트 위젯 설정
    }

    // 2. ProgressBar 생성
    if (!PbHpBar)
    {
        PbHpBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("PbHpBar"));
        if (PbHpBar)
        {
            // 3. ProgressBar를 VerticalBox에 추가
            UVerticalBoxSlot* VerticalBoxSlot = RootVerticalBox->AddChildToVerticalBox(PbHpBar);
            if (VerticalBoxSlot)
            {
                // 4. 레이아웃 설정
                VerticalBoxSlot->SetPadding(FMargin(10.0f));  // 여백 설정
                VerticalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill); // 수평 정렬
                VerticalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);   // 수직 정렬
            }
        }
    }

    // 디버깅을 위한 로그 추가
    if (!PbHpBar)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create ProgressBar"));
    }
}