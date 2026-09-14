#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace GitHubRepos
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchDirectories =
	{
		".git"sv,
		".github"sv
	};

	static constexpr std::array HardMatchDirectorySubStrings =
	{
		"-main"sv,
		"-master"sv
	};

	inline bool MatchesDirectoryRules( const std::filesystem::path& Directory )
	{
		const std::string DirectoryName = Directory.filename().string();

		auto SubStringPredicate = [&DirectoryName] ( const std::string_view SubString )
		{
			return DirectoryName.contains( SubString );
		};

		if ( std::ranges::any_of( HardMatchDirectorySubStrings, SubStringPredicate ) ) return true;

		for ( const auto& Entry : std::filesystem::directory_iterator( Directory ) )
		{
			if ( !Entry.is_directory() ) continue;

			const std::string EntryName = Entry.path().filename().string();
			if ( std::ranges::contains( HardMatchDirectories, DirectoryName ) ) return true;
		}

		return false;
	}

	inline bool IsFileNameHardMatch( const std::filesystem::path& File )
	{
		std::string FileName = File.filename().string();
		std::ranges::transform( FileName.begin(), FileName.end(), FileName.begin(), [] ( const unsigned char C ) { return std::tolower( C ); } );

		auto SubStringPredicate = [&FileName] ( const std::string_view SubString )
		{
			return FileName.contains( SubString );
		};

		return std::ranges::any_of( HardMatchDirectorySubStrings, SubStringPredicate );
	}

	inline bool MatchesFileRules( const std::filesystem::path& Path )
	{
		return IsFileNameHardMatch( Path );
	}

	inline bool MatchesRules( const std::filesystem::path& Path )
	{
		if ( !exists(Path) ) return false;

		const bool IsDirectory = std::filesystem::is_directory( Path );
		const bool IsFile      = std::filesystem::is_regular_file( Path );

		if ( IsDirectory && MatchesDirectoryRules( Path ) ) return true;
		if ( IsFile && MatchesFileRules( Path ) ) return true;

		return false;
	}
}
