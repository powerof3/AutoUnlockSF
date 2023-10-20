#pragma once

#include "RE.h"
#include "Settings.h"

namespace AutoUnlock
{
	bool CanAutoUnlock(const Setting& a_setting, const RE::REFR_LOCK* a_lockData, const RE::TESObjectREFR* a_owner);

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
					const auto formSetting = settings->GetSetting(FormType);

				    if (CanAutoUnlock(formSetting, lock, activatedRef)) {
						auto lockLevel = lock->GetLockLevel(activatedRef);
						if (lockLevel == RE::LOCK_LEVEL::kInaccessible) {
							lockLevel = static_cast<RE::LOCK_LEVEL>(formSetting.unlockInaccessible);
						}

				        RE::RemoveItem(RE::PlayerCharacter::GetSingleton(), { RE::TESForm::LookupByID(0xA), nullptr }, 1);

						a_data.activatedRef->Unlock();

						RE::LockPickedEvent::Notify(RE::NiPointer(actionRef), RE::NiPointer(activatedRef), true, true, lockLevel, 1);

						if (settings->playSoundOnUnlock) {
							RE::PlayMenuSound("UI_Menu_Minigame_Security_Puzzle_Success");
						}

						const auto lockpickXP = RE::GetLockpickXPReward(lockLevel);
						RE::RewardXP(RE::PlayerCharacter::GetSingleton(), lockpickXP, 4, true, activatedRef);

						// TODO - figure out crime bounties/terminal events
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
