// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FontEditorModule.h"
#include "MouseDeltaTracker.h"
#include "SFontEditorViewport.h"
#include "Runtime/Engine/Public/Slate/SceneViewport.h"

#define LOCTEXT_NAMESPACE "FontEditor"


/*-----------------------------------------------------------------------------
   FFontEditorViewportClient
-----------------------------------------------------------------------------*/

class FFontEditorViewportClient : public FViewportClient
{
public:
	/** Constructor */
	FFontEditorViewportClient(TWeakPtr<SFontEditorViewport> InFontEditorViewport);

	/** FLevelEditorViewportClient interface */
	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override;
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.f, bool bGamepad = false) override;
	
	/** Determines which texture page was selected. */
	void UpdateSelectedPage(UObject* SelectedObject);

	/** Accessors */
	int32 GetCurrentSelectedPage() const;
	void SetPreviewText(const FText& InPreviewText);
	void SetBackgroundColor(const FColor& InBackgroundColor);
	const FColor& GetBackgroundColor() const;
	void SetForegroundColor(const FColor& InForgroundColor);
	const FColor& GetForegroundColor() const;
	
	/** Returns the ratio of the size of the font texture to the size of the viewport */
	float GetViewportVerticalScrollBarRatio() const;
	float GetViewportHorizontalScrollBarRatio() const;

private:
	/** Updates the states of the scrollbars */
	void UpdateScrollBars();

	/** Changes the position of the vertical scrollbar (on a mouse scrollwheel event) */
	void ChangeViewportScrollBarPosition(EScrollDirection Direction);

	/** Returns the positions of the scrollbars relative to the font textures */
	FVector2D GetViewportScrollBarPositions() const;

private:
	/** Pointer back to the Font viewport control that owns us */
	TWeakPtr<SFontEditorViewport> FontEditorViewportPtr;

	/** Which font page is currently selected */
	int32 CurrentSelectedPage;

	/** Text displayed in font preview viewports */
	FText PreviewText;

	/** Background and foreground color used by font preview viewports */
	FColor BackgroundColor;
	FColor ForegroundColor;

	/** The size of the gap between pages */
	const int32 PageGap;
};

FFontEditorViewportClient::FFontEditorViewportClient(TWeakPtr<SFontEditorViewport> InFontEditorViewport)
	: FontEditorViewportPtr(InFontEditorViewport)
	, CurrentSelectedPage(INDEX_NONE)
	, PageGap(4)
{
	PreviewText = LOCTEXT("DefaultPreviewText", "The quick brown fox jumped over the lazy dog");
	BackgroundColor = FColor::Black;
	ForegroundColor = FColor::White;
}

void FFontEditorViewportClient::Draw(FViewport* Viewport, FCanvas* Canvas)
{
	UFont* Font = FontEditorViewportPtr.Pin()->GetFontEditor().Pin()->GetFont();
	
	if (!FontEditorViewportPtr.Pin()->IsPreviewViewport())
	{
		FVector2D Ratio = FVector2D(GetViewportHorizontalScrollBarRatio(), GetViewportVerticalScrollBarRatio());
		FVector2D ViewportSize = FVector2D(FontEditorViewportPtr.Pin()->GetViewport()->GetSizeXY().X, FontEditorViewportPtr.Pin()->GetViewport()->GetSizeXY().Y);
		FVector2D ScrollBarPos = GetViewportScrollBarPositions();
		int32 YOffset = (Ratio.Y > 1.0f)? ((ViewportSize.Y - (ViewportSize.Y / Ratio.Y)) * 0.5f): 0;
		int32 YPos = YOffset - ScrollBarPos.Y;
		int32 LastDrawnYPos = YPos;
		int32 XOffset = (Ratio.X > 1.0f)? ((ViewportSize.X - (ViewportSize.X / Ratio.X)) * 0.5f): 0;
		int32 XPos = XOffset - ScrollBarPos.X;
	
		UpdateScrollBars();

		Canvas->Clear( FColor(0,0,0));

		// Draw checkerbox background
		const static float BoxSize = 40.0f;
		bool XColor = false;
		bool YColor = false;
		FVector2D BackgroundPos;
		for (BackgroundPos.Y = 0.0f; BackgroundPos.Y <= ViewportSize.Y; BackgroundPos.Y += BoxSize)
		{
			YColor = !YColor;
			XColor = YColor;
			for (BackgroundPos.X = 0.0f; BackgroundPos.X <= ViewportSize.X; BackgroundPos.X += BoxSize)
			{
				XColor = !XColor;
				Canvas->DrawTile( BackgroundPos.X, BackgroundPos.Y, BoxSize, BoxSize, 0.0f, 0.0f, 1.0f, 1.0f, XColor? FLinearColor::Gray: FLinearColor(0.25f, 0.25f, 0.25f));
			}
		}

		// Loop through the pages drawing them if they are visible
		for (int32 Index = 0; Index < Font->Textures.Num(); ++Index)
		{
			UTexture* Texture = Font->Textures[Index];
			// Make sure it's a valid texture page. Could be null if the user
			// is editing things
			if (Texture != NULL)
			{
				// Get the rendering info for this object
				FThumbnailRenderingInfo* RenderInfo = GUnrealEd->GetThumbnailManager()->GetRenderingInfo(Texture);
				// If there is an object configured to handle it, draw the thumbnail
				if (RenderInfo != NULL && RenderInfo->Renderer != NULL)
				{
					uint32 Width, Height;

					// Figure out the size we need
					RenderInfo->Renderer->GetThumbnailSize(Texture, 1.0f, Width, Height);

					// Don't draw if we are outside of our range
					if (YPos + Height >= 0 && YPos <= (int32)Viewport->GetSizeXY().Y)
					{
						// If hit testing, draw a tile instead
						if (Canvas->IsHitTesting())
						{
							Canvas->SetHitProxy(new HObject(Texture));
						
							// Draw a simple tile
							Canvas->DrawTile( XPos, YPos, Width, Height, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White, GEditor->BkgndHi->Resource);
						
							Canvas->SetHitProxy(NULL);
						}
						// Otherwise draw the font texture
						else
						{
							// Draw a selected background
							if (Texture->IsSelected())
							{
								Canvas->DrawTile( XPos, YPos, Width, Height, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White, GEditor->BkgndHi->Resource);
							}
							else
							{
								Canvas->DrawTile( XPos, YPos, Width, Height, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::Black);
							}

							// Draw the font texture (with alpha blending enabled)
							Canvas->DrawTile( XPos, YPos, Width, Height, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White, Texture->Resource, true);
						}
					}
					// Update our total height and current draw position
					YPos += Height + PageGap;
				}
			}
		}
	}
	else
	{
		// Erase with our background color
		Canvas->Clear( BackgroundColor);

		// And draw the text with the foreground color
		FCanvasTextItem TextItem( FVector2D::ZeroVector, PreviewText, Font, FLinearColor(ForegroundColor) );
		Canvas->DrawItem( TextItem );		
	}
}

bool FFontEditorViewportClient::InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool Gamepad)
{
	// The preview viewport doesn't need to process key input
	if (FontEditorViewportPtr.Pin()->IsPreviewViewport())
	{
		return false;
	}

	bool bHandled = false;

	if (Key == EKeys::LeftMouseButton && Event == IE_Released)
	{
		Viewport->Invalidate();
		bHandled = true;
	}
	else
	{
		if((Key == EKeys::LeftMouseButton || Key == EKeys::RightMouseButton) && Event == IE_Pressed)
		{
			const int32 HitX = Viewport->GetMouseX();
			const int32 HitY = Viewport->GetMouseY();
			// See if we hit something
			HHitProxy* HitResult = Viewport->GetHitProxy(HitX,HitY);
			if (HitResult)
			{
				if (HitResult->IsA(HObject::StaticGetType()))
				{
					// Get the object that was hit
					UObject* HitObject = ((HObject*)HitResult)->Object;
					if (HitObject)
					{
						// Turn off all others and set it as selected
						if (HitObject->IsA(AActor::StaticClass()))
						{
							//UE_LOG(LogFontPropDlg, Warning, TEXT("WxFontPropertiesDlg::InputKey : selecting actor!"));
							GEditor->GetSelectedActors()->DeselectAll();
							GEditor->GetSelectedActors()->Select(HitObject);
						}
						else
						{
							//UE_LOG(LogFontPropDlg, Log, TEXT("WxFontPropertiesDlg::InputKey : selecting object!"));
							GEditor->GetSelectedObjects()->DeselectAll();
							GEditor->GetSelectedObjects()->Select(HitObject);
						}
						// Update our internal state for selected page
						// buttons, etc.
						UpdateSelectedPage(HitObject);
					}
				}
			}
		
			// Force a redraw
			Viewport->Invalidate();
			Viewport->InvalidateDisplay();

			bHandled = true;
		}
		// Did they scroll using the mouse wheel?
		else if (Key == EKeys::MouseScrollUp)
		{
			ChangeViewportScrollBarPosition(Scroll_Down);
			bHandled = true;
		}
		// Did they scroll using the mouse wheel?
		else if (Key == EKeys::MouseScrollDown)
		{
			ChangeViewportScrollBarPosition(Scroll_Up);
			bHandled = true;
		}
	}
	
	return bHandled;
}

void FFontEditorViewportClient::UpdateSelectedPage(UObject* SelectedObject)
{
	UFont* Font = FontEditorViewportPtr.Pin()->GetFontEditor().Pin()->GetFont();

	// Default to non-selected
	CurrentSelectedPage = INDEX_NONE;

	// Search through the font's texture array seeing if this is a match
	for (int32 Index = 0; Index < Font->Textures.Num() && CurrentSelectedPage == INDEX_NONE; Index++)
	{
		// See if the pointers match
		if (Font->Textures[Index] == SelectedObject)
		{
			CurrentSelectedPage = Index;
			break;
		}
	}

	FontEditorViewportPtr.Pin()->GetFontEditor().Pin()->SetSelectedPage(CurrentSelectedPage);
}

int32 FFontEditorViewportClient::GetCurrentSelectedPage() const
{
	return CurrentSelectedPage;
}

void FFontEditorViewportClient::SetPreviewText(const FText& InPreviewText)
{
	PreviewText = InPreviewText;
}

void FFontEditorViewportClient::SetBackgroundColor(const FColor& InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
}

const FColor& FFontEditorViewportClient::GetBackgroundColor() const
{
	return BackgroundColor;
}

void FFontEditorViewportClient::SetForegroundColor(const FColor& InForegroundColor)
{
	ForegroundColor = InForegroundColor;
}

const FColor& FFontEditorViewportClient::GetForegroundColor() const
{
	return ForegroundColor;
}

float FFontEditorViewportClient::GetViewportVerticalScrollBarRatio() const
{
	float WidgetHeight = 1.0f;
	float TextureHeight = 1.0f;
	if (FontEditorViewportPtr.Pin()->GetVerticalScrollBar().IsValid())
	{
		UFont* Font = FontEditorViewportPtr.Pin()->GetFontEditor().Pin()->GetFont();
		
		WidgetHeight = FontEditorViewportPtr.Pin()->GetViewport()->GetSizeXY().Y;
		
		for (int32 Idx = 0; Idx < Font->Textures.Num(); ++Idx)
		{
			FThumbnailRenderingInfo* RenderInfo = GUnrealEd->GetThumbnailManager()->GetRenderingInfo(Font->Textures[Idx]);
			if (RenderInfo != NULL && RenderInfo->Renderer != NULL)
			{
				uint32 Width, Height;
				RenderInfo->Renderer->GetThumbnailSize(Font->Textures[Idx], 1.0f, Width, Height);
				TextureHeight += Height;
			}
		}

		TextureHeight += (Font->Textures.Num() - 1) * PageGap;
	}

	return WidgetHeight / TextureHeight;
}

float FFontEditorViewportClient::GetViewportHorizontalScrollBarRatio() const
{
	uint32 Width = 1;
	float WidgetWidth = 1.0f;
	if (FontEditorViewportPtr.Pin()->GetHorizontalScrollBar().IsValid())
	{
		UFont* Font = FontEditorViewportPtr.Pin()->GetFontEditor().Pin()->GetFont();
		uint32 Height = 1;

		WidgetWidth = FontEditorViewportPtr.Pin()->GetViewport()->GetSizeXY().X;

		if(Font->Textures.Num())
		{
			FThumbnailRenderingInfo* RenderInfo = GUnrealEd->GetThumbnailManager()->GetRenderingInfo(Font->Textures[0]);
			if (RenderInfo != NULL && RenderInfo->Renderer != NULL)
			{
				RenderInfo->Renderer->GetThumbnailSize(Font->Textures[0], 1.0f, Width, Height);
			}
		}

	}

	return WidgetWidth / Width;
}

void FFontEditorViewportClient::UpdateScrollBars()
{
	if (FontEditorViewportPtr.Pin()->GetVerticalScrollBar().IsValid() && FontEditorViewportPtr.Pin()->GetHorizontalScrollBar().IsValid())
	{
		float VRatio = GetViewportVerticalScrollBarRatio();
		float HRatio = GetViewportHorizontalScrollBarRatio();
		float VDistFromBottom = FontEditorViewportPtr.Pin()->GetVerticalScrollBar()->DistanceFromBottom();
		float HDistFromBottom = FontEditorViewportPtr.Pin()->GetHorizontalScrollBar()->DistanceFromBottom();

		if (VRatio < 1.0f)
		{
			if (VDistFromBottom < 1.0f)
			{
				FontEditorViewportPtr.Pin()->GetVerticalScrollBar()->SetState(FMath::Clamp(1.0f - VRatio - VDistFromBottom, 0.0f, 1.0f), VRatio);
			}
			else
			{
				FontEditorViewportPtr.Pin()->GetVerticalScrollBar()->SetState(0.0f, VRatio);
			}
		}

		if (HRatio < 1.0f)
		{
			if (HDistFromBottom < 1.0f)
			{
				FontEditorViewportPtr.Pin()->GetHorizontalScrollBar()->SetState(FMath::Clamp(1.0f - HRatio - HDistFromBottom, 0.0f, 1.0f), HRatio);
			}
			else
			{
				FontEditorViewportPtr.Pin()->GetHorizontalScrollBar()->SetState(0.0f, HRatio);
			}
		}
	}
}

void FFontEditorViewportClient::ChangeViewportScrollBarPosition(EScrollDirection Direction)
{
	if (FontEditorViewportPtr.Pin()->GetVerticalScrollBar().IsValid())
	{
		float Ratio = GetViewportVerticalScrollBarRatio();
		float DistFromBottom = FontEditorViewportPtr.Pin()->GetVerticalScrollBar()->DistanceFromBottom();
		float OneMinusRatio = 1.0f - Ratio;
		float Diff = 0.1f * OneMinusRatio;

		if (Direction == Scroll_Down)
		{
			Diff *= -1.0f;
		}

		FontEditorViewportPtr.Pin()->GetVerticalScrollBar()->SetState(FMath::Clamp(OneMinusRatio - DistFromBottom + Diff, 0.0f, OneMinusRatio), Ratio);

		FontEditorViewportPtr.Pin()->GetViewport()->Invalidate();
		FontEditorViewportPtr.Pin()->GetViewport()->InvalidateDisplay();
	}
}

FVector2D FFontEditorViewportClient::GetViewportScrollBarPositions() const
{
	FVector2D Positions = FVector2D::ZeroVector;
	if (FontEditorViewportPtr.Pin()->GetVerticalScrollBar().IsValid() && FontEditorViewportPtr.Pin()->GetHorizontalScrollBar().IsValid())
	{
		UFont* Font = FontEditorViewportPtr.Pin()->GetFontEditor().Pin()->GetFont();
		float VRatio = GetViewportVerticalScrollBarRatio();
		float HRatio = GetViewportHorizontalScrollBarRatio();
		float VDistFromBottom = FontEditorViewportPtr.Pin()->GetVerticalScrollBar()->DistanceFromBottom();
		float HDistFromBottom = FontEditorViewportPtr.Pin()->GetHorizontalScrollBar()->DistanceFromBottom();
	
		if ((FontEditorViewportPtr.Pin()->GetVerticalScrollBar()->GetVisibility() == EVisibility::Visible) && VDistFromBottom < 1.0f)
		{
			float TextureHeight = 0.0f;
			for (int32 Idx = 0; Idx < Font->Textures.Num(); ++Idx)
			{
				FThumbnailRenderingInfo* RenderInfo = GUnrealEd->GetThumbnailManager()->GetRenderingInfo(Font->Textures[Idx]);
				if (RenderInfo != NULL && RenderInfo->Renderer != NULL)
				{
					uint32 Width, Height;
					RenderInfo->Renderer->GetThumbnailSize(Font->Textures[Idx], 1.0f, Width, Height);
					TextureHeight += Height;
				}
			}

			TextureHeight += (Font->Textures.Num() - 1) * PageGap;

			Positions.Y = FMath::Clamp(1.0f - VRatio - VDistFromBottom, 0.0f, 1.0f) * TextureHeight;
		}
		else
		{
			Positions.Y = 0.0f;
		}

		if ((FontEditorViewportPtr.Pin()->GetHorizontalScrollBar()->GetVisibility() == EVisibility::Visible) && HDistFromBottom < 1.0f)
		{
			uint32 Width = 0;
			uint32 Height = 0;
			FThumbnailRenderingInfo* RenderInfo = GUnrealEd->GetThumbnailManager()->GetRenderingInfo(Font->Textures[0]);
			if (RenderInfo != NULL && RenderInfo->Renderer != NULL)
			{
				RenderInfo->Renderer->GetThumbnailSize(Font->Textures[0], 1.0f, Width, Height);
			}

			Positions.X = FMath::Clamp(1.0f - HRatio - HDistFromBottom, 0.0f, 1.0f) * Width;
		}
		else
		{
			Positions.X = 0.0f;
		}
	}

	return Positions;
}


SFontEditorViewport::~SFontEditorViewport()
{

}

void SFontEditorViewport::Construct(const FArguments& InArgs)
{
	FontEditorPtr = InArgs._FontEditor;
	bIsPreview = InArgs._IsPreview;

	this->ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.FillHeight(1)
				[
					SAssignNew(ViewportWidget, SViewport)
					.EnableGammaCorrection(false)
					.IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute())
					.ShowEffectWhenDisabled(false)
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(FontViewportVerticalScrollBar, SScrollBar)
				.Visibility(this, &SFontEditorViewport::GetViewportVerticalScrollBarVisibility)
				.OnUserScrolled(this, &SFontEditorViewport::OnViewportVerticalScrollBarScrolled)
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(FontViewportHorizontalScrollBar, SScrollBar)
			.Orientation( Orient_Horizontal )
			.Visibility(this, &SFontEditorViewport::GetViewportHorizontalScrollBarVisibility)
			.OnUserScrolled(this, &SFontEditorViewport::OnViewportHorizontalScrollBarScrolled)
		]
	];

	ViewportClient = MakeShareable(new FFontEditorViewportClient(SharedThis(this)));

	Viewport = MakeShareable(new FSceneViewport(ViewportClient.Get(), ViewportWidget));

	// The viewport widget needs an interface so it knows what should render
	ViewportWidget->SetViewportInterface( Viewport.ToSharedRef() );
}

void SFontEditorViewport::RefreshViewport()
{
	Viewport->Invalidate();
	Viewport->InvalidateDisplay();
}

int32 SFontEditorViewport::GetCurrentSelectedPage() const
{
	if (ViewportClient.IsValid())
	{
		return ViewportClient->GetCurrentSelectedPage();
	}

	return INDEX_NONE;
}

void SFontEditorViewport::SetPreviewText(const FText& PreviewText)
{
	if (ViewportClient.IsValid())
	{
		ViewportClient->SetPreviewText(PreviewText);

		RefreshViewport();
	}
}

void SFontEditorViewport::SetPreviewBackgroundColor(const FColor& BackgroundColor)
{
	if (ViewportClient.IsValid())
	{
		ViewportClient->SetBackgroundColor(BackgroundColor);

		RefreshViewport();
	}
}

const FColor& SFontEditorViewport::GetPreviewBackgroundColor() const
{
	if (ViewportClient.IsValid())
	{
		return ViewportClient->GetBackgroundColor();
	}

	return FColor::Black;
}

void SFontEditorViewport::SetPreviewForegroundColor(const FColor& ForegroundColor)
{
	if (ViewportClient.IsValid())
	{
		ViewportClient->SetForegroundColor(ForegroundColor);

		RefreshViewport();
	}
}

const FColor& SFontEditorViewport::GetPreviewForegroundColor() const
{
	if (ViewportClient.IsValid())
	{
		return ViewportClient->GetForegroundColor();
	}

	return FColor::White;
}

TWeakPtr<IFontEditor> SFontEditorViewport::GetFontEditor() const
{
	return FontEditorPtr;
}

bool SFontEditorViewport::IsPreviewViewport() const
{
	return bIsPreview;
}

TSharedPtr<FSceneViewport> SFontEditorViewport::GetViewport() const
{
	return Viewport;
}

TSharedPtr<SViewport> SFontEditorViewport::GetViewportWidget() const
{
	return ViewportWidget;
}

TSharedPtr<SScrollBar> SFontEditorViewport::GetVerticalScrollBar() const
{
	return FontViewportVerticalScrollBar;
}

TSharedPtr<SScrollBar> SFontEditorViewport::GetHorizontalScrollBar() const
{
	return FontViewportHorizontalScrollBar;
}

EVisibility SFontEditorViewport::GetViewportVerticalScrollBarVisibility() const
{
	return (!bIsPreview && (ViewportClient->GetViewportVerticalScrollBarRatio() < 1.0f))? EVisibility::Visible: EVisibility::Collapsed;
}

EVisibility SFontEditorViewport::GetViewportHorizontalScrollBarVisibility() const
{
	return (!bIsPreview && (ViewportClient->GetViewportHorizontalScrollBarRatio() < 1.0f))? EVisibility::Visible: EVisibility::Collapsed;
}

void SFontEditorViewport::OnViewportVerticalScrollBarScrolled(float InScrollOffsetFraction)
{
	float Ratio = ViewportClient->GetViewportVerticalScrollBarRatio();
	float MaxOffset = (Ratio < 1.0f)? 1.0f - Ratio: 0.0f;
	InScrollOffsetFraction = FMath::Clamp(InScrollOffsetFraction, 0.0f, MaxOffset);
	FontViewportVerticalScrollBar->SetState(InScrollOffsetFraction, Ratio);
	RefreshViewport();
}

void SFontEditorViewport::OnViewportHorizontalScrollBarScrolled(float InScrollOffsetFraction)
{
	float Ratio = ViewportClient->GetViewportHorizontalScrollBarRatio();
	float MaxOffset = (Ratio < 1.0f)? 1.0f - Ratio: 0.0f;
	InScrollOffsetFraction = FMath::Clamp(InScrollOffsetFraction, 0.0f, MaxOffset);
	FontViewportHorizontalScrollBar->SetState(InScrollOffsetFraction, Ratio);
	RefreshViewport();
}

#undef LOCTEXT_NAMESPACE
