#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace Videos
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".mp4"sv,
		".m4v"sv,
		".mov"sv,
		".avi"sv,
		".mkv"sv,
		".webm"sv,
		".wmv"sv,
		".flv"sv,
		".f4v"sv,
		".mpeg"sv,
		".mpg"sv,
		".mpe"sv,
		".mpv"sv,
		".m2v"sv,
		".m2ts"sv,
		".mts"sv,
		".ts"sv,
		".vob"sv,
		".3gp"sv,
		".3g2"sv,
		".ogv"sv
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
