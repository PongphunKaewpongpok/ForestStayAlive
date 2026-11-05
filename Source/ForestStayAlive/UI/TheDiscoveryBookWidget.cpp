// Fill out your copyright notice in the Description page of Project Settings.


#include "TheDiscoveryBookWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "ForestStayAlive/DiscoveryBookManager.h"
#include "ForestStayAlive/DataAssets/DiscoveryBookData.h"
#include "ForestStayAlive/MySaveGame.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"

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
	
	ShowAllImageButton->OnClicked.AddDynamic(
		this,
		&UTheDiscoveryBookWidget::OnShowAllImageClicked
	);
	
	
	UpdatePage();
}

void UTheDiscoveryBookWidget::UpdateCaptureImage(const FString& DiscoveryID)
{
    if (!IsValid(CaptureImage)) return;

    if (!IsValid(DiscoveryBookManager))
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    UMySaveGame* SaveGame = DiscoveryBookManager->GetSaveData();

    if (!IsValid(SaveGame))
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }
	
	
	
    const FPhotoData* PhotoData = SaveGame->FindPhotoData(DiscoveryID);

    if (!PhotoData || PhotoData->FilePaths.IsEmpty())
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    FString FilePath = PhotoData->FilePaths[0];

    if (FPaths::IsRelative(FilePath))
    {
        FilePath = FPaths::Combine(
            FPaths::ScreenShotDir(),
            FilePath
        );
    }

    if (!FPaths::FileExists(FilePath))
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    TArray<uint8> CompressedData;
    if (!FFileHelper::LoadFileToArray(CompressedData, *FilePath))
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    if (CompressedData.IsEmpty())
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    IImageWrapperModule& ImageWrapperModule =
        FModuleManager::LoadModuleChecked<IImageWrapperModule>(
            FName("ImageWrapper")
        );

    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (!ImageWrapper.IsValid())
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    if (!ImageWrapper->SetCompressed(
        CompressedData.GetData(),
        CompressedData.Num()
    ))
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    TArray64<uint8> RawData;

    if (!ImageWrapper->GetRaw(
        ERGBFormat::BGRA,
        8,
        RawData
    ))
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    const int32 Width = ImageWrapper->GetWidth();
    const int32 Height = ImageWrapper->GetHeight();

    if (Width <= 0 || Height <= 0 || RawData.IsEmpty())
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    UTexture2D* Texture = UTexture2D::CreateTransient(
        Width,
        Height,
        PF_B8G8R8A8
    );

    if (!IsValid(Texture))
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    FTexturePlatformData* PlatformData = Texture->GetPlatformData();

    if (!PlatformData || PlatformData->Mips.IsEmpty())
    {
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    FTexture2DMipMap& Mip = PlatformData->Mips[0];

    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);

    if (!TextureData)
    {
        Mip.BulkData.Unlock();
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    const int64 ExpectedSize = static_cast<int64>(Width) * Height * 4;

    if (RawData.Num() != ExpectedSize)
    {
        Mip.BulkData.Unlock();
        if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
        return;
    }

    FMemory::Memcpy(
        TextureData,
        RawData.GetData(),
        ExpectedSize
    );

    Mip.BulkData.Unlock();

    Texture->SRGB = true;
    Texture->UpdateResource();

    CaptureTexture = Texture;
    CaptureImage->SetBrushFromTexture(CaptureTexture);
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

	const FName DiscoveryID = Data->DiscoveryID;

	const bool bNameUnlocked = DiscoveryBookManager->IsDiscoveryNameUnlocked(DiscoveryID);
	
	if (DiscoveryName)
	{
		DiscoveryName->SetText(bNameUnlocked ? Data->Name : FText::FromString(TEXT("???")));
	}
	
	if (bNameUnlocked)
	{
		UpdateCaptureImage(DiscoveryID.ToString());
	}
	else if (CaptureImage)
	{
		if (LockImageTexture) CaptureImage->SetBrushFromTexture(LockImageTexture);
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

void UTheDiscoveryBookWidget::OnShowAllImageClicked()
{
	// To Be Continue...
}
