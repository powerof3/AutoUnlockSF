#include "Hooks.h"
#include "Settings.h"

#include "RE/B/BGSInventoryItem.h"
#include "RE/E/ExtraLock.h"

namespace RE
{
	void RemoveItem(TESObjectREFR* a_refr, const BGSObjectInstance& a_object, std::uint32_t a_count)
	{
		constexpr std::uint32_t handle = 0;

		using func_t = void (*)(TESObjectREFR*, const std::uint32_t&, const BGSObjectInstance&, std::uint32_t);
		REL::Relocation<func_t> func{ REL::ID(106863) };
		return func(a_refr, handle, a_object, a_count);
	}
}

namespace AutoUnlock
{
	namespace detail
	{
		bool player_has_digipick()
		{
			bool hasDigipick = false;

			RE::PlayerCharacter::GetSingleton()->ForEachInventoryItem([&](const RE::BGSInventoryItem& a_item) {
				if (a_item.object->IsDigiPick()) {
					hasDigipick = true;
					return RE::BSContainer::ForEachResult::kStop;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});

			return hasDigipick;
		}
	}

	bool CanLockpick(const Setting& a_setting, const RE::REFR_LOCK* a_lockData, const RE::TESObjectREFR* a_owner)
	{
		if (!a_lockData->IsLocked() || !detail::player_has_digipick()) {
			return false;
		}

		const auto lockLevel = a_lockData->GetLockLevel(a_owner);

		float entryPoint = 0.0f;
		RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kGetPlayerGateHacking, RE::PlayerCharacter::GetSingleton(), std::addressof(entryPoint));
		const auto playerLockpickingLevel = static_cast<RE::LOCK_LEVEL>(entryPoint);

		switch (lockLevel) {
		case RE::LOCK_LEVEL::kEasy:
		case RE::LOCK_LEVEL::kAverage:
		case RE::LOCK_LEVEL::kHard:
		case RE::LOCK_LEVEL::kVeryHard:
			return a_setting.autoUnlock && playerLockpickingLevel >= lockLevel;
		case RE::LOCK_LEVEL::kInaccessible:
			return a_setting.unlockInaccessible != -1 && playerLockpickingLevel >= static_cast<RE::LOCK_LEVEL>(a_setting.unlockInaccessible);
		default:
			return false;
		}
	}

	void InstallOnPostLoad()
	{
		Settings::GetSingleton()->Load();

		Activate<RE::FormType::kDOOR>::Install(RE::VTABLE::TESObjectDOOR[6]);
		Activate<RE::FormType::kCONT>::Install(RE::VTABLE::TESObjectCONT[12]);
		Activate<RE::FormType::kTERM>::Install(RE::VTABLE::BGSTerminal[11]);
	}
}
