#pragma once

namespace RE
{
	// structs

	namespace stl::local_function_detail
	{
		class LocalFunctionImpl
		{
		public:
			SF_HEAP_REDEFINE_NEW(LocalFunctionImpl);
			SF_RTTI_VTABLE(stl__local_function_detail__LocalFunctionImpl__lambda_9c77057997406db74c04b5f37fd1f509__BSEventNotifyControl_BSTEventDetail__SinkBase_);

			LocalFunctionImpl(void* a_event, void* a_eventSource) :
				event(a_event),
				eventSource(a_eventSource)
			{
				REX::EMPLACE_VTABLE(this);
			}

			virtual ~LocalFunctionImpl() = default;

			// members
			void* event;
			void* eventSource;
		};
	}

	class BGSStoryEventManager
	{
	public:
		[[nodiscard]] static BGSStoryEventManager* GetSingleton()
		{
			using func_t = decltype(&BGSStoryEventManager::GetSingleton);
			static REL::Relocation<func_t> func{ REL::ID(62173) };  // DONE
			return func();
		}

		std::uint32_t AddEvent(std::uint32_t a_index, const void* a_event)
		{
			using func_t = std::uint32_t (BGSStoryEventManager::*)(std::uint32_t, const void*);
			static REL::Relocation<func_t> func{ REL::ID(72962) };  // DONE
			return func(this, a_index, a_event);
		}

		template <class T>
		std::uint32_t AddEvent(const T& a_event)
		{
			return AddEvent(T::EVENT_INDEX(), &a_event);
		}
	};

	inline std::uint32_t* CreatePointerHandle(std::uint32_t* a_handleOut, TESForm* a_form)
	{
		using func_t = decltype(&CreatePointerHandle);
		static REL::Relocation<func_t> func{ REL::ID(36247) };  // DONE
		return func(a_handleOut, a_form);
	}

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
			REL::Relocation<std::uint32_t*> eventIdx{ REL::ID(810248) };  // DONE
			return *eventIdx;
		}

		// members
		std::uint32_t actorHandle;  // 00
		std::uint32_t lockObject;   // 04
		std::uint32_t isCrime;      // 08
	};
	static_assert(sizeof(BGSPickLockEvent) == 0x0C);

	// functions

	inline bool HasKey(TESObjectREFR* a_refr, TESObjectREFR* a_activateRef, std::uint32_t a_arg3, REFR_LOCK* a_data)
	{
		using func_t = decltype(&HasKey);
		static REL::Relocation<func_t> func{ REL::ID(100830) };  // DONE
		return func(a_refr, a_activateRef, a_arg3, a_data);
	}

	inline std::uint32_t* RemoveItem(TESObjectREFR* a_refr, const BGSObjectInstance& a_object, std::uint32_t a_count)
	{
		constexpr std::uint32_t handle = 0;
		using func_t = std::uint32_t* (*)(TESObjectREFR*, const std::uint32_t&, const BGSObjectInstance&, std::uint32_t);
		static REL::Relocation<func_t> func{ REL::ID(63586) };
		return func(a_refr, handle, a_object, a_count);
	}

	inline float GetLockpickXPReward(LOCK_LEVEL a_lockLevel)
	{
		using func_t = decltype(&GetLockpickXPReward);
		static REL::Relocation<func_t> func{ REL::ID(69501) };  // DONE
		return func(a_lockLevel);
	}

	inline void SendLockPickingEvent(
		TESObjectREFR*   a_actionRef,
		TESObjectREFR*   a_lock,
		BGSTerminalMenu* a_menu,
		bool             a_succeeded,
		bool             a_isOffLimits,
		LOCK_LEVEL       lockLevel,
		std::uint32_t    a_numDigiPicksUsed)
	{
		auto lockPickingEvent = RE::LockPickedEvent::GetEventSource();

		RE::LockPickedEvent event{};
		event.actionRef = NiPointer(a_actionRef);
		event.lock = NiPointer(a_lock);
		event.terminalMenu = a_menu;
		event.unk18 = -1;
		event.numDigiPicksUsed = a_numDigiPicksUsed;
		event.unk20 = 0.0f;
		event.unk24 = 0.0f;
		event.lockLevel = lockLevel;
		event.succeeded = a_succeeded;
		event.isOffLimits = a_isOffLimits;

		stl::local_function_detail::LocalFunctionImpl helper(&event, lockPickingEvent);
		lockPickingEvent->Notify(&helper);
	}
}
