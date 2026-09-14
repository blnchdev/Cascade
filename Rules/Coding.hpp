#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace Coding
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".c"sv,
		".cc"sv,
		".cpp"sv,
		".cxx"sv,
		".h"sv,
		".hh"sv,
		".hpp"sv,
		".hxx"sv,

		".ixx"sv,
		".cppm"sv,

		".rs"sv,
		".go"sv,
		".zig"sv,
		".java"sv,
		".kt"sv,
		".kts"sv,
		".swift"sv,
		".m"sv,
		".mm"sv,

		".cs"sv,
		".fs"sv,
		".fsx"sv,
		".vb"sv,

		".asm"sv,
		".s"sv,

		".py"sv,
		".pyw"sv,
		".js"sv,
		".jsx"sv,
		".ts"sv,
		".tsx"sv,
		".lua"sv,
		".rb"sv,
		".php"sv,
		".pl"sv,
		".ps1"sv,
		".psm1"sv,

		".sh"sv,
		".bash"sv,
		".zsh"sv,
		".fish"sv,
		".bat"sv,
		".cmd"sv,

		".sql"sv,
		".cmake"sv,
		".toml"sv,
		".gradle"sv,
		".gradle.kts"sv
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

	inline bool MatchesDirectoryRule( const std::filesystem::path& Directory )
	{
		auto Predicate = [] ( const std::filesystem::directory_entry& Entry )
		{
			const bool IsDirectory = std::filesystem::is_directory( Entry );
			const bool IsFile      = Entry.is_regular_file();

			if ( IsFile && MatchesFileRule( Entry ) ) return true;
			if ( IsDirectory && MatchesDirectoryRule( Entry ) ) return true;

			return false;
		};

		return std::ranges::any_of( std::filesystem::directory_iterator( Directory ), Predicate );
	}

	inline bool MatchesRules( const std::filesystem::path& Path )
	{
		if ( !exists(Path) ) return false;

		const bool IsDirectory = std::filesystem::is_directory( Path );
		const bool IsFile      = std::filesystem::is_regular_file( Path );

		if ( IsFile && MatchesFileRule( Path ) ) return true;
		if ( IsDirectory && MatchesDirectoryRule( Path ) ) return true;

		return false;
	}
}
