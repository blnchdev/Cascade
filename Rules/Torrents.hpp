#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace Torrents
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".torrent"sv,
		".fastresume"sv,
		".resume"sv,
		".torrentinfo"sv
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
		const bool Exists = exists( Path );

		if ( !Exists ) return false;

		return MatchesFileRule( Path );
	}
}
