#pragma once

#include "RE/B/BGSInventoryItem.h"
#include "RE/E/ExtraLock.h"
#include "RE/L/LockPickedEvent.h"
#include "RE/M/Misc.h"

namespace RE
{
	inline void RemoveItem(TESObjectREFR* a_refr, const BGSObjectInstance& a_object, std::uint32_t a_count)
	{
		constexpr std::uint32_t handle = 0;

		using func_t = void (*)(TESObjectREFR*, const std::uint32_t&, const BGSObjectInstance&, std::uint32_t);
		REL::Relocation<func_t> func{ REL::ID(106863) };
		return func(a_refr, handle, a_object, a_count);
	}

	inline float GetLockpickXPReward(LOCK_LEVEL a_lockLevel)
	{
		using func_t = decltype(&GetLockpickXPReward);
		REL::Relocation<func_t> func{ REL::ID(113775) };
		return func(a_lockLevel);
	}

	inline void RewardXP(Actor* a_player, float a_xpExperience, std::uint32_t a_type, bool a_useModifiers, TESObjectREFR* a_refr, std::uint64_t a_arg6 = 0, std::uint64_t a_arg7 = 0)
	{
		using func_t = decltype(&RewardXP);
		REL::Relocation<func_t> func{ REL::ID(151231) };
		return func(a_player, a_xpExperience, a_type, a_useModifiers, a_refr, a_arg6, a_arg7);
	}
}
