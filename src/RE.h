#pragma once

#include "RE/T/TESKey.h"

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

	inline std::uint32_t* CreatePointerHandle(std::uint32_t* a_handleOut, TESForm* a_form)
	{
		using func_t = decltype(&CreatePointerHandle);
		REL::Relocation<func_t> func{ REL::ID(72975) };
		return func(a_handleOut, a_form);
	}

	class BGSStoryEventManager
	{
	public:
		[[nodiscard]] static BGSStoryEventManager* GetSingleton()
		{
			using func_t = decltype(&BGSStoryEventManager::GetSingleton);
			REL::Relocation<func_t> func{ REL::ID(37526) };
			return func();
		}

		std::uint32_t AddEvent(std::uint32_t a_index, const void* a_event)
		{
			using func_t = std::uint32_t (BGSStoryEventManager::*)(std::uint32_t, const void*);
			REL::Relocation<func_t> func{ REL::ID(118074) };
			return func(this, a_index, a_event);
		}

		template <class T>
		std::uint32_t AddEvent(const T& a_event)
		{
			return AddEvent(T::EVENT_INDEX(), &a_event);
		}
	};

	struct BGSPickLockEvent
	{
	public:
		BGSPickLockEvent(TESObjectREFR* a_actor, TESObjectREFR* a_lockRef)
		{
			std::uint32_t handleActorOut{ 0 };
			std::uint32_t handleLockOut{ 0 };

			actorHandle = *CreatePointerHandle(&handleActorOut, a_actor);
			lockObject = *CreatePointerHandle(&handleLockOut, a_actor);
			isCrime = a_lockRef->IsCrimeToActivate();
		}

		[[nodiscard]] static std::uint32_t EVENT_INDEX()
		{
			REL::Relocation<std::uint32_t*> eventIdx{ REL::ID(762531) };
			return *eventIdx;
		}

		// members
		std::uint32_t actorHandle;  // 00
		std::uint32_t lockObject;   // 04
		std::uint32_t isCrime;      // 08
	};
	static_assert(sizeof(BGSPickLockEvent) == 0x0C);
}
