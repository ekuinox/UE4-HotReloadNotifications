// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HotReloadNotifications.h"
#include "HttpModule.h"
#include "Developer/HotReload/Public/IHotReload.h"
#include "Interfaces/IHttpRequest.h"
#include "Serialization/JsonSerializerMacros.h"

#define LOCTEXT_NAMESPACE "FHotReloadNotificationsModule"

static constexpr auto DISCORD_WEBHOOK_URL = "";

struct FDiscordWebhookRequestBody : public FJsonSerializable
{
	FString Content;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("content", Content);
	END_JSON_SERIALIZER

	FDiscordWebhookRequestBody(const FString& Content) : Content(Content) {}
	FDiscordWebhookRequestBody() : FDiscordWebhookRequestBody(FString()) {}
};

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

	const auto CompilationResultToString = [](ECompilationResult::Type CompilationResult) -> TCHAR * {
		static constexpr TCHAR* ToString[] = {
			L"Successed",
			L"Canceled",
			L"UpToDate",
			L"CrashOrAssert",
			L"FailedDueToHeaderChange",
			L"OtherCompilationError",
			L"Unsupported",
			L"Unknown"
		};
		static constexpr auto Length = sizeof(ToString) / sizeof(ToString[0]);
		
		const auto Index = static_cast<uint8>(CompilationResult);

		if (Index >= Length) {
			return L"Unknown";
		}

		return ToString[Index];
	};

	const auto Body = FDiscordWebhookRequestBody(FString(L"[HotReloadNotifications] Compile Finished with ") + CompilationResultToString(CompilationResult));

	auto Request = FHttpModule::Get().CreateRequest();

	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetURL(FString(DISCORD_WEBHOOK_URL));
	Request->SetContentAsString(Body.ToJson());

	Request->ProcessRequest();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHotReloadNotificationsModule, HotReloadNotifications)