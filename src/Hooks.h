#pragma once

#include "RE.h"
#include "Settings.h"

namespace AutoUnlock
{
	bool CanAutoUnlock(const Setting& a_setting, RE::REFR_LOCK* a_lockData, RE::TESObjectREFR* a_owner);

	template <class T>
	struct Activate
	{
		static void thunk(std::uintptr_t a_this, RE::OBJ_ACTIVATE_DATA& a_data)
		{
			const auto activatedRef = a_data.activatedRef;
			const auto actionRef = a_data.actionRef;

			if (activatedRef && actionRef && actionRef->IsPlayerRef()) {
				if (const auto lock = activatedRef->GetLock()) {
					const auto  settings = Settings::GetSingleton();
				    const auto& formSetting = settings->GetSetting(T::FORMTYPE);

					if (CanAutoUnlock(formSetting, lock, activatedRef)) {
						auto lockLevel = lock->GetLockLevel(activatedRef);
						if (lockLevel == RE::LOCK_LEVEL::kInaccessible) {
							lockLevel = static_cast<RE::LOCK_LEVEL>(formSetting.unlockInaccessible);
						}
						// play sound
						if (settings->playSoundOnUnlock) {
							RE::PlayMenuSound("UI_Menu_Minigame_Security_Puzzle_Success");
						}

						// award XP
						const auto lockpickXP = RE::GetLockpickXPReward(lockLevel);
						RE::RewardXP(RE::PlayerCharacter::GetSingleton(), lockpickXP, 4, true, activatedRef);

						// unlock + send TESLockChangedEvent
						activatedRef->Unlock();

						// send LockPickEvent
						RE::LockPickedEvent::Notify(RE::NiPointer(actionRef), RE::NiPointer(activatedRef), true, activatedRef->IsCrimeToActivate(), lockLevel, 1);

						// send story event
						const RE::BGSPickLockEvent storyEvent(actionRef, activatedRef);
						RE::BGSStoryEventManager::GetSingleton()->AddEvent(storyEvent);

						// remove digipick (ignore ModLockpickRewardKeyChance entry point)
						RE::RemoveItem(RE::PlayerCharacter::GetSingleton(), { RE::TESForm::LookupByID(0xA), nullptr }, 1);

						// TODO - figure out crime bounties
					}
				}
			}

			return func(a_this, a_data);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t            idx{ 0x54 };

		static void Install(std::size_t a_idx)
		{
			stl::write_vfunc<T, Activate>(a_idx);
			logger::info("Installed {} Activate hook"sv, typeid(T).name());
		}
	};

	void InstallOnPostLoad();
}
