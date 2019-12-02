// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HotReloadNotifications.h"
#include "Developer/HotReload/Public/IHotReload.h"

#define LOCTEXT_NAMESPACE "FHotReloadNotificationsModule"

void FHotReloadNotificationsModule::StartupModule()
{
	const auto& HotReloadModule = IHotReloadInterface::GetPtr();

	HotReloadModule->OnModuleCompilerFinished().AddRaw(this, &FHotReloadNotificationsModule::OnCompileFinished);
}

void FHotReloadNotificationsModule::ShutdownModule()
{
}

void FHotReloadNotificationsModule::OnCompileFinished(const FString& LogDump, ECompilationResult::Type CompilationResult, bool bShowLog) const
{
	UE_LOG(LogTemp, Log, TEXT("[HotReloadNotifications] %s"), *LogDump);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHotReloadNotificationsModule, HotReloadNotifications)