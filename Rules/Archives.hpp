#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace Archives
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".zip"sv,
		".rar"sv,
		".7z"sv,
		".tar"sv,
		".gz"sv,
		".bz2"sv,
		".xz"sv,
		".z"sv,

		".tgz"sv,
		".tbz"sv,
		".tbz2"sv,
		".txz"sv,

		".arj"sv,
		".lzh"sv,
		".lha"sv,
		".cab"sv,
		".cpio"sv,

		".iso"sv,
		".img"sv,
		".dmg"sv,
		".vhd"sv,
		".vhdx"sv,
		".vmdk"sv,

		".zst"sv,
		".lz"sv,
		".lz4"sv,
		".lzo"sv,
		".br"sv,
		".sz"sv,

		".jar"sv,
		".war"sv,
		".ear"sv
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
