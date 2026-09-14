#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace Audios
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".mp3"sv,
		".wav"sv,
		".flac"sv,
		".ogg"sv,
		".oga"sv,
		".opus"sv,
		".m4a"sv,
		".aac"sv,
		".wma"sv,
		".aiff"sv,
		".aif"sv,
		".aifc"sv,
		".alac"sv,
		".amr"sv,
		".ape"sv,
		".ac3"sv,
		".dts"sv,
		".mka"sv,
		".mid"sv,
		".midi"sv,
		".rmi"sv,
		".wv"sv,
		".au"sv,
		".snd"sv
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
