#pragma once
#include <algorithm>
#include <filesystem>

namespace Libraries
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".lib"sv,
		".a"sv,
		".la"sv,

		".dll"sv,
		".so"sv,
		".dylib"sv,

		".ocx"sv,
		".sys"sv,
		".drv"sv
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
