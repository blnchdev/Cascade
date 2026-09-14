#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace Documents
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".doc"sv,
		".docx"sv,
		".docm"sv,
		".dot"sv,
		".dotx"sv,
		".dotm"sv,
		".xls"sv,
		".xlsx"sv,
		".xlsm"sv,
		".xlsb"sv,
		".xlt"sv,
		".xltx"sv,
		".xltm"sv,
		".ppt"sv,
		".pptx"sv,
		".pptm"sv,
		".pot"sv,
		".potx"sv,
		".potm"sv,

		".odt"sv,
		".ott"sv,
		".ods"sv,
		".ots"sv,
		".odp"sv,
		".otp"sv,

		".pdf"sv,
		".epub"sv,
		".mobi"sv,
		".azw"sv,
		".azw3"sv,
		".fb2"sv,
		".djvu"sv,

		".txt"sv,
		".rtf"sv,
		".md"sv,
		".markdown"sv,
		".csv"sv,
		".tsv"sv,
		".log"sv,
		".tex"sv,
		".latex"sv,

		".html"sv,
		".htm"sv,
		".xhtml"sv,
		".xml"sv,
		".json"sv,
		".yaml"sv,
		".yml"sv,

		".pages"sv,
		".numbers"sv,
		".key"sv,

		".wps"sv,
		".wpd"sv,
		".odg"sv,
		".sxw"sv,
		".sxc"sv,
		".sxi"sv
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
