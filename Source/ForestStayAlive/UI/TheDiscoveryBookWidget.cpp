// Fill out your copyright notice in the Description page of Project Settings.


#include "TheDiscoveryBookWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "ForestStayAlive/DiscoveryBookManager.h"
#include "ForestStayAlive/DataAssets/DiscoveryBookData.h"

void UTheDiscoveryBookWidget::ClearDetailBoxes()
{
	if (DiscoveryName) DiscoveryName->SetText(FText::FromString(TEXT("???")));
	
	if (AppearancesBox) AppearancesBox->ClearChildren();
	if (BenefitsBox) BenefitsBox->ClearChildren();
	if (PrecautionsBox) PrecautionsBox->ClearChildren();
	if (FoundInLocationsBox) FoundInLocationsBox->ClearChildren();
}

void UTheDiscoveryBookWidget::AddDetailText(UVerticalBox* TargetBox, const FText& Text)
{
	if (!TargetBox || !TemplateDetail) return;

	UTextBlock* NewText = DuplicateObject<UTextBlock>(TemplateDetail.Get(), WidgetTree.Get());

	if (!NewText) return;
	NewText->SetText(Text);

	TargetBox->AddChildToVerticalBox(NewText);
}

void UTheDiscoveryBookWidget::UpdateDetailSection(
	UVerticalBox* TargetBox,
	const TMap<FName, FText>& Details,
	EDiscoveryBookSection Section,
	FName DiscoveryID
)
{
	if (!TargetBox) return;

	for (const auto& Detail : Details)
	{
		const FName ItemID = Detail.Key;
		const FText DetailText = Detail.Value;

		const bool bUnlocked = DiscoveryBookManager->IsDiscoveryItemUnlocked(DiscoveryID, Section, ItemID);

		AddDetailText(
			TargetBox,
			bUnlocked ? DetailText : FText::FromString(TEXT("- ???"))
		);
	}
}



void UTheDiscoveryBookWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (TemplateDetail) TemplateDetail->RemoveFromParent();
	
	
	DiscoveryBookManager = GetGameInstance()->GetSubsystem<UDiscoveryBookManager>();
	
	
	PreviousPage->OnClicked.AddDynamic(
		this,
		&UTheDiscoveryBookWidget::OnPreviousPageClicked
	);
	NextPage->OnClicked.AddDynamic(
		this,
		&UTheDiscoveryBookWidget::OnNextPageClicked
	);
	
	PlantsButton->OnClicked.AddDynamic(
		this,
		&UTheDiscoveryBookWidget::OnPlantsButtonClicked
	);
	AnimalsButton->OnClicked.AddDynamic(
		this,
		&UTheDiscoveryBookWidget::OnAnimalsButtonClicked
	);
	
	
	UpdatePage();
}


void UTheDiscoveryBookWidget::OnPlantsButtonClicked()
{
	Category = FName("Plants");
	
	UpdatePage();
}

void UTheDiscoveryBookWidget::OnAnimalsButtonClicked()
{
	Category = FName("Animals");
	
	UpdatePage();
}

void UTheDiscoveryBookWidget::UpdatePage()
{
	if (!DiscoveryBookManager) return;

	ClearDetailBoxes();

	UDiscoveryBookData* Data = DiscoveryBookManager->GetDiscoveryDataByIndex(Category, PageIndex);

	if (!Data) return;

	const FName DiscoveryID = Data->GetFName();

	const bool bNameUnlocked = DiscoveryBookManager->IsDiscoveryNameUnlocked(DiscoveryID);
	
	if (DiscoveryName)
	{
		DiscoveryName->SetText(bNameUnlocked ? Data->Name : FText::FromString(TEXT("???")));
	}
	
	
	UpdateDetailSection(
		AppearancesBox,
		Data->Appearances,
		EDiscoveryBookSection::Appearances,
		DiscoveryID
	);

	UpdateDetailSection(
		BenefitsBox,
		Data->Benefits,
		EDiscoveryBookSection::Benefits,
		DiscoveryID
	);

	UpdateDetailSection(
		PrecautionsBox,
		Data->Precautions,
		EDiscoveryBookSection::Precautions,
		DiscoveryID
	);

	UpdateDetailSection(
		FoundInLocationsBox,
		Data->FoundInLocations,
		EDiscoveryBookSection::FoundInLocations,
		DiscoveryID
	);
}

void UTheDiscoveryBookWidget::OnPreviousPageClicked()
{
	if (!DiscoveryBookManager) return;
	
	PageIndex--;
	PageIndex = FMath::Clamp(
		PageIndex,
		0,
		DiscoveryBookManager->GetDiscoveryDataSize(Category)-1
	);
	
	UpdatePage();
}

void UTheDiscoveryBookWidget::OnNextPageClicked()
{
	if (!DiscoveryBookManager) return;
	
	PageIndex++;
	PageIndex = FMath::Clamp(
		PageIndex,
		0,
		DiscoveryBookManager->GetDiscoveryDataSize(Category)-1
	);
	
	UpdatePage();
}
