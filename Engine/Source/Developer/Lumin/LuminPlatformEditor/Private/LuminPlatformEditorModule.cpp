#include "LuminTargetSettingsDetails.h"
#include "LuminRuntimeSettings.h"
#include "ModuleInterface.h"
#include "ISettingsModule.h"
#include "ModuleManager.h"
#include "MaterialShaderQualitySettingsCustomization.h"
#include "MaterialShaderQualitySettings.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "RenderingThread.h"
#include "ComponentRecreateRenderStateContext.h"
#include "MagicLeapSDKSettings.h"
#include "IAndroidDeviceDetectionModule.h"
#include "PropertyEditorModule.h"
#include "ShaderPlatformQualitySettings.h"

#define LOCTEXT_NAMESPACE "FLuminPlatformEditorModule"


/**
 * Module for Lumin platform editor utilities
 */
class FLuminPlatformEditorModule
	: public IModuleInterface
{
	// IModuleInterface interface

	virtual void StartupModule() override
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		static FName LuminTargetSettings("LuminRuntimeSettings");
		PropertyModule.RegisterCustomClassLayout(LuminTargetSettings, FOnGetDetailCustomizationInstance::CreateStatic(&FLuminTargetSettingsDetails::MakeInstance));

		FOnUpdateMaterialShaderQuality UpdateMaterials = FOnUpdateMaterialShaderQuality::CreateLambda([]()
		{
			FGlobalComponentRecreateRenderStateContext Recreate;
			FlushRenderingCommands();
			UMaterial::AllMaterialsCacheResourceShadersForRendering();
			UMaterialInstance::AllMaterialsCacheResourceShadersForRendering();
		});

		PropertyModule.RegisterCustomClassLayout(
			UShaderPlatformQualitySettings::StaticClass()->GetFName(),
			FOnGetDetailCustomizationInstance::CreateStatic(&FMaterialShaderQualitySettingsCustomization::MakeInstance, UpdateMaterials)
			);

		PropertyModule.NotifyCustomizationModuleChanged();

		// register settings
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			{
				SettingsModule->RegisterSettings("Project", "Platforms", "Lumin",
					LOCTEXT("RuntimeSettingsName", "Magic Leap"),
					LOCTEXT("RuntimeSettingsDescription", "Project settings for MagicLeap apps"),
					GetMutableDefault<ULuminRuntimeSettings>()
				);
			}

			{
				SettingsModule->RegisterSettings("Project", "Platforms", "MagicLeapSDK",
					LOCTEXT("SDKSettingsName", "Magic Leap SDK"),
					LOCTEXT("SDKSettingsDescription", "Settings for Magic Leap SDK (for all projects)"),
					GetMutableDefault<UMagicLeapSDKSettings>()
				);
			}

			// @todo Shader quality settings
		}

		// Force the SDK settings into a sane state initially so we can make use of them
		auto &TargetPlatformManagerModule = FModuleManager::LoadModuleChecked<ITargetPlatformManagerModule>("TargetPlatform");
		UMagicLeapSDKSettings* settings = GetMutableDefault<UMagicLeapSDKSettings>();
		settings->SetTargetModule(&TargetPlatformManagerModule);
		auto &LuminDeviceDetection = FModuleManager::LoadModuleChecked<IAndroidDeviceDetectionModule>("AndroidDeviceDetection");
		settings->SetDeviceDetection(LuminDeviceDetection.GetAndroidDeviceDetection(TEXT("Lumin")));
		settings->UpdateTargetModulePaths();
	}

	virtual void ShutdownModule() override
	{
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			SettingsModule->UnregisterSettings("Project", "Platforms", "Lumin");
			SettingsModule->UnregisterSettings("Project", "Platforms", "MagicLeapSDK");
		}
	}
};


IMPLEMENT_MODULE(FLuminPlatformEditorModule, LuminPlatformEditor);

#undef LOCTEXT_NAMESPACE
