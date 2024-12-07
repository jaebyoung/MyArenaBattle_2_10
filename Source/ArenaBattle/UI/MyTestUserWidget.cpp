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

    // ������� ���� �α� �߰�
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
        // ���α׷����� ������ �Ķ������� ����
        PbHpBar->SetFillColorAndOpacity(FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));

        // ���α׷����ٸ� 50%�� ����
        PbHpBar->SetPercent(0.5f);
    }
}

void UMyTestUserWidget::CreateHpBar()
{
    // 1. ���� VerticalBox�� �ִ��� Ȯ���ϰ� ������ ����
    UVerticalBox* RootVerticalBox = Cast<UVerticalBox>(GetRootWidget());
    if (!RootVerticalBox)
    {
        // Root VerticalBox ����
        RootVerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootVerticalBox"));
        WidgetTree->RootWidget = RootVerticalBox; // ��Ʈ ���� ����
    }

    // 2. ProgressBar ����
    if (!PbHpBar)
    {
        PbHpBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("PbHpBar"));
        if (PbHpBar)
        {
            // 3. ProgressBar�� VerticalBox�� �߰�
            UVerticalBoxSlot* VerticalBoxSlot = RootVerticalBox->AddChildToVerticalBox(PbHpBar);
            if (VerticalBoxSlot)
            {
                // 4. ���̾ƿ� ����
                VerticalBoxSlot->SetPadding(FMargin(10.0f));  // ���� ����
                VerticalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill); // ���� ����
                VerticalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);   // ���� ����
            }
        }
    }

    // ������� ���� �α� �߰�
    if (!PbHpBar)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create ProgressBar"));
    }
}