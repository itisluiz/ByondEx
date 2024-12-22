#include <process/identity.hh>
#include <Windows.h>
#include <filesystem>
#include <optional>

namespace process::identity
{
	namespace fs = std::filesystem;

	static std::optional<fs::path> getModulePath()
	{
		wchar_t buffer[MAX_PATH];
		if (!GetModuleFileNameW(GetModuleHandleW(NULL), buffer, sizeof(buffer) / sizeof(wchar_t)))
			return { };

		return { buffer };
	}

	ByondIdentity getByondIdentity()
	{
		static std::optional<ByondIdentity> identity;

		if (identity)
			return *identity;

		std::optional<fs::path> modulePath{ getModulePath() };

		if (!modulePath)
		{
			identity = ByondIdentity::kUnknown;
			return *identity;
		}

		std::wstring filename{ modulePath->filename().wstring() };

		if (filename == L"byond.exe")
			identity = ByondIdentity::kByond;
		else if (filename == L"dreamseeker.exe")
			identity = ByondIdentity::kDreamSeeker;
		else if (filename == L"dreamdaemon.exe")
			identity = ByondIdentity::kDreamDaemon;
		else if (filename == L"dreammaker.exe")
			identity = ByondIdentity::kDreamMaker;
		else
			identity = ByondIdentity::kUnknown;

		return *identity;
	}
}
