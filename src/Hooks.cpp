#include "Hooks.h"
#include "Settings.h"

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

		bool can_unlock(const std::uint32_t a_playerLockpickLevel, std::uint32_t a_lockLevel, bool a_oneRankHigher)
		{
			if (a_oneRankHigher) {
				a_lockLevel += 1;
			}
			return a_playerLockpickLevel >= a_lockLevel;
		}
	}

	bool CanAutoUnlock(const Setting& a_setting, const RE::REFR_LOCK* a_lockData, const RE::TESObjectREFR* a_owner)
	{
		if (!a_lockData->IsLocked() || !detail::player_has_digipick()) {
			return false;
		}

		float entryPoint = 0.0f;
		RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kGetPlayerGateHacking, RE::PlayerCharacter::GetSingleton(), std::addressof(entryPoint));
        const auto playerLockpickingLevel = static_cast<std::uint32_t>(entryPoint);

		switch (const auto lockLevel = a_lockData->GetLockLevel(a_owner)) {
		case RE::LOCK_LEVEL::kEasy:
		case RE::LOCK_LEVEL::kAverage:
		case RE::LOCK_LEVEL::kHard:
		case RE::LOCK_LEVEL::kVeryHard:
			return a_setting.autoUnlock != -1 && detail::can_unlock(playerLockpickingLevel, std::to_underlying(lockLevel), a_setting.autoUnlock == 1);
		case RE::LOCK_LEVEL::kInaccessible:
			return a_setting.unlockInaccessible != -1 && detail::can_unlock(playerLockpickingLevel, a_setting.unlockInaccessible, a_setting.autoUnlock == 1);
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
