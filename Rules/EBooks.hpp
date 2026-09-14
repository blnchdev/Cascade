#pragma once
#include <algorithm>
#include <filesystem>

namespace EBooks
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".epub"sv,
		".mobi"sv,
		".azw"sv,
		".azw3"sv,
		".azw4"sv,
		".kf8"sv,
		".kfx"sv,

		".fb2"sv,
		".lit"sv,
		".lrf"sv,
		".prc"sv,

		".djvu"sv,
		".djv"sv,

		".cbz"sv,
		".cbr"sv,
		".cb7"sv,
		".cbt"sv,

		".ibooks"sv,
		".opf"sv,

		".tcr"sv
	};

	inline bool IsExtensionHardMatch( const std::filesystem::path& Extension )
	{
		std::string String = Extension.string();
		std::ranges::transform( String.begin(), String.end(), String.begin(), [] ( const unsigned char C ) { return std::tolower( C ); } );

		return std::ranges::contains( HardMatchExtensions, String );
	}

	inline bool MatchesFileRule( const std::filesystem::path& File ) { return File.has_extension() && IsExtensionHardMatch( File.extension() ); }

	inline bool MatchesRules( const std::filesystem::path& Path )
	{
		if ( !exists( Path ) ) return false;

		return MatchesFileRule( Path );
	}
}
