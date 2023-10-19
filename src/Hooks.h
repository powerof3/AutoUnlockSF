#pragma once

#include "RE/M/Misc.h"
#include "Settings.h"

namespace RE
{
	void RemoveItem(TESObjectREFR* a_refr, const BGSObjectInstance& a_object, std::uint32_t a_count);
}

namespace AutoUnlock
{
    namespace detail
	{
		bool player_has_digipick();
	}

	bool CanLockpick(const Setting& a_setting, const RE::REFR_LOCK* a_lockData, const RE::TESObjectREFR* a_owner);

	template <RE::FormType FormType>
	struct Activate
	{
		static void thunk(std::uintptr_t a_this, RE::OBJ_ACTIVATE_DATA& a_data)
		{
			const auto activatedRef = a_data.activatedRef;
			const auto actionRef = a_data.actionRef;

			if (activatedRef && actionRef && actionRef->IsPlayerRef()) {
				if (const auto lock = activatedRef->GetLock()) {
					const auto settings = Settings::GetSingleton();
					if (CanLockpick(settings->GetSetting(FormType), lock, activatedRef)) {
						RE::RemoveItem(RE::PlayerCharacter::GetSingleton(), { RE::TESForm::LookupByID(0xA), nullptr }, 1);
						if (settings->playSoundOnUnlock) {
							RE::PlayMenuSound("UI_Menu_Minigame_Security_Puzzle_Success");
						}
						a_data.activatedRef->Unlock();
					}
				}
			}

			return func(a_this, a_data);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t            idx{ 0x54 };

		static void Install(REL::ID a_id)
		{
			stl::write_vfunc<Activate>(a_id);
			logger::info("Installed {} Activate hook"sv, FormType);
		}
	};

	void InstallOnPostLoad();
}
