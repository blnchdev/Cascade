#pragma once
#include <algorithm>
#include <filesystem>

namespace SoftwareInstallers
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".exe"sv,
		".msi"sv,
		".msix"sv,
		".msixbundle"sv,
		".appx"sv,
		".appxbundle"sv,

		".msp"sv,
		".mst"sv,

		".msu"sv,
		".cab"sv,

		".dmg"sv,
		".pkg"sv,

		".deb"sv,
		".rpm"sv,
		".apk"sv,

		".mpkg"sv,

		".jar"sv,

		".whl"sv,
		".egg"sv,

		".snap"sv,
		".flatpak"sv,
		".appimage"sv,
	};

	inline bool IsExtensionHardMatch( const std::filesystem::path& Extension )
	{
		std::string String = Extension.string();
		std::ranges::transform( String.begin(), String.end(), String.begin(), [] ( const unsigned char C ) { return std::tolower( C ); } );

		return std::ranges::contains( HardMatchExtensions, String );
	}

	inline bool MatchesFileRule( const std::filesystem::path& File )
	{
		return File.has_extension() && IsExtensionHardMatch( File.extension() );
	}

	inline bool MatchesRules( const std::filesystem::path& Path )
	{
		if ( !exists(Path) ) return false;

		return MatchesFileRule( Path );
	}
}
