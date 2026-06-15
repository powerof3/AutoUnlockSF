#include "Hooks.h"

AutoUnlock::Activate<RE::TESObjectDOOR,4> activateDoor;
AutoUnlock::Activate<RE::TESObjectCONT,5> activateCont;
AutoUnlock::Activate<RE::BGSTerminal,6> activateTerm;

void MessageHandler(SFSE::MessagingInterface::Message* a_message)
{
	switch (a_message->type) {
	case SFSE::MessagingInterface::kPostLoad:
		AutoUnlock::InstallOnPostLoad();
		break;
	default:
		break;
	}
}

SFSE_EXPORT constinit auto SFSEPlugin_Version = []() noexcept {
	SFSE::PluginVersionData data{};

	data.PluginVersion(Version::MAJOR);
	data.PluginName(Version::PROJECT);
	data.AuthorName("powerofthree");
	data.UsesSigScanning(false);
	data.UsesAddressLibrary(true);
	data.HasNoStructUse(false);
	data.IsLayoutDependent(true);
	data.CompatibleVersions({ SFSE::RUNTIME_LATEST });

	return data;
}();

SFSE_PLUGIN_LOAD(const SFSE::LoadInterface* a_sfse)
{
	SFSE::Init(a_sfse, { .trampoline = false });
	SFSE::GetMessagingInterface()->RegisterListener(MessageHandler);
	return true;
}
