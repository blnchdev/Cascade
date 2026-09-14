#pragma once
#include <algorithm>
#include <filesystem>

namespace Symbols
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".pdb"sv,
		".idb"sv,
		".ilk"sv,
		".id0"sv,
		".id1"sv,
		".id2"sv,
		".i64"sv,

		".dbg"sv,
		".dmp"sv,
		".mdmp"sv,

		".map"sv,
		".sym"sv,

		".obj"sv,
		".o"sv,
		".lib"sv,
		".exp"sv,

		".pch"sv,
		".ipch"sv,

		".sbr"sv,
		".bsc"sv,

		".tds"sv,
		".res"sv,

		".gdb"sv,
		".dwo"sv,
		".dwp"sv,

		".elf"sv,
		".axf"sv,
		".out"sv,
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
