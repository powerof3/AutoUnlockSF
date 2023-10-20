#pragma once

struct Setting
{
	void Load(const char* a_type, CSimpleIniA& a_ini, bool a_writeComments)
	{
		ini::get_value(a_ini, autoUnlock, a_type, "iAutoUnlockMode", a_writeComments ? ";Automatically unlock objects if you have the Security perk.\n;-1 : disabled, 0 : autounlock if perk rank is equal to or greater than lock level, 1 : autounlock if perk rank is greater than lock level" : nullptr);
		ini::get_value(a_ini, unlockInaccessible, a_type, "iUnlockInaccessibleLevel", a_writeComments ? ";Unlock inaccessible objects.\n;-1 : disabled, 0-3 : required Security perk rank" : nullptr);
	}

	// members
	std::int32_t autoUnlock{ 0 };
	std::int32_t unlockInaccessible{ -1 };
};

class Settings : public ISingleton<Settings>
{
public:
	void Load()
	{
		constexpr auto path = L"Data/SFSE/Plugins/po3_AutoUnlock.ini";

		CSimpleIniA ini;
		ini.SetUnicode();

		ini.LoadFile(path);

		ini::get_value(ini, playSoundOnUnlock, "Settings", "bPlaySoundOnUnlock", nullptr);

		door.Load("Door", ini, true);
		container.Load("Container", ini, false);
		terminal.Load("Terminal", ini, false);

		(void)ini.SaveFile(path);
	}

	[[nodiscard]] const Setting& GetSetting(const RE::FormType a_type) const
	{
		if (a_type == RE::FormType::kDOOR) {
			return door;
		}
		if (a_type == RE::FormType::kCONT) {
			return container;
		}
		return terminal;
	}

	bool playSoundOnUnlock{ true };

	Setting door;
	Setting container;
	Setting terminal;
};
