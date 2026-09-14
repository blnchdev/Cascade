#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

#include "Rules/Archives.hpp"
#include "Rules/Audios.hpp"
#include "Rules/Coding.hpp"
#include "Rules/DiskImages.hpp"
#include "Rules/Documents.hpp"
#include "Rules/EBooks.hpp"
#include "Rules/Fonts.hpp"
#include "Rules/GitHubRepos.hpp"
#include "Rules/Images.hpp"
#include "Rules/Libraries.hpp"
#include "Rules/SoftwareInstallers.hpp"
#include "Rules/Symbols.hpp"
#include "Rules/Torrents.hpp"
#include "Rules/Videos.hpp"

namespace
{
	using RuleMatcher = bool(*)( const std::filesystem::path& );

	enum class EType : uint16_t
	{
		Coding,
		SoftwareInstallers,
		Audios,
		Videos,
		Images,
		EBooks,
		Documents,
		Archives,
		Symbols,
		Torrents,
		GitHubRepos,
		Fonts,
		DiskImages,
		Libraries
	};

	enum class EPriority : uint16_t
	{
		VeryLow,
		Low,
		Medium,
		High,
		VeryHigh
	};

	struct Config
	{
		std::filesystem::path Downloads;
		std::filesystem::path Target;
		bool                  IsDryRun     = false;
		bool                  IsReOrganize = false;
	};

	struct RuleDefinition
	{
		std::string_view DirectoryName;
		RuleMatcher      Predicate;
		uint32_t         Index;
		EPriority        Priority;
		EType            Value;

		constexpr explicit RuleDefinition( const EType Value, const std::string_view DirectoryName, const RuleMatcher Predicate, const uint32_t Index = 0, const EPriority Priority = EPriority::Medium ) : DirectoryName( DirectoryName ), Predicate( Predicate ), Index( Index ), Priority( Priority ), Value( Value )
		{
			if ( this->Index == 0 )
			{
				this->Index = std::to_underlying( Value );
			}
		}

		[[nodiscard]] std::string GetFormattedDirectoryName() const
		{
			return std::format( "{:02X} - {}", this->Index, this->DirectoryName );
		}
	};

	[[nodiscard]] consteval auto MakeRules()
	{
		std::array Rules
		{
			RuleDefinition( EType::Coding, "Coding", Coding::MatchesRules, 0x10, EPriority::High ),
			RuleDefinition( EType::GitHubRepos, "GitHub Repositories", GitHubRepos::MatchesRules, 0x11, EPriority::VeryHigh ),

			RuleDefinition( EType::SoftwareInstallers, "Software Installers", SoftwareInstallers::MatchesRules, 0x20, EPriority::High ),

			RuleDefinition( EType::Videos, "Videos", Videos::MatchesRules, 0x30, EPriority::High ),
			RuleDefinition( EType::Images, "Images", Images::MatchesRules, 0x31, EPriority::VeryHigh ),
			RuleDefinition( EType::Fonts, "Fonts", Fonts::MatchesRules, 0x32, EPriority::Medium ),
			RuleDefinition( EType::Audios, "Audios", Audios::MatchesRules, 0x33, EPriority::High ),
			RuleDefinition( EType::EBooks, "E-Books", EBooks::MatchesRules, 0x34, EPriority::Medium ),

			RuleDefinition( EType::Documents, "Documents", Documents::MatchesRules, 0x40, EPriority::Medium ),
			RuleDefinition( EType::Archives, "Archives", Archives::MatchesRules, 0x41, EPriority::Medium ),
			RuleDefinition( EType::Torrents, "Torrents", Torrents::MatchesRules, 0x42, EPriority::VeryLow ),
			RuleDefinition( EType::DiskImages, "Disk Images", DiskImages::MatchesRules, 0x43, EPriority::High ),
			RuleDefinition( EType::Symbols, "Debug Symbols", Symbols::MatchesRules, 0x44, EPriority::Low ),
			RuleDefinition( EType::Libraries, "Libraries", Libraries::MatchesRules, 0x45, EPriority::Low )
		};

		std::ranges::sort( Rules, std::ranges::greater{}, &RuleDefinition::Priority );

		return Rules;
	}

	constexpr auto Rules = MakeRules();

	void ShowProgress( std::string_view From, std::string_view To, float Fraction, bool& IsFirst, const int32_t Width = 30 )
	{
		Fraction = std::clamp( Fraction, 0.0f, 1.0f );

		const auto Percent = static_cast<int32_t>( Fraction * 100.0f );
		const auto Filled  = static_cast<int32_t>( Fraction * static_cast<float>( Width ) );

		std::string Bar( Width, ' ' );

		for ( int32_t i = 0; i < Filled; ++i ) Bar[ i ] = '=';

		const auto PercentText = std::format( "{}%", Percent );
		const auto PercentPos  = ( Width - static_cast<int32_t>( PercentText.size() ) ) / 2;

		for ( std::size_t i = 0; i < PercentText.size(); ++i ) Bar[ PercentPos + static_cast<int32_t>( i ) ] = PercentText[ i ];

		if ( !IsFirst ) std::print( "\033[1A\r" );

		IsFirst = false;

		std::print( "\033[2KMoving {} to {}\n", From, To );
		std::print( "\033[2K[{}]", Bar );

		std::fflush( stdout );
	}

	// Little hack but since we consteval RuleDefinition for a constexpr ordering of Rules, we create a wrapper struct that
	// contains runtime information, i.e. Path
	struct RuntimeRule
	{
		const RuleDefinition& Rule;
		std::filesystem::path Path;
	};

	std::vector<RuntimeRule> RuntimeRules;

	const RuntimeRule* GetType( const std::filesystem::path& Path )
	{
		auto Predicate = [&Path] ( const RuntimeRule& Runtime ) { return Runtime.Rule.Predicate && Runtime.Rule.Predicate( Path ); };

		const auto Iterator = std::ranges::find_if( RuntimeRules, Predicate );
		if ( Iterator == RuntimeRules.end() ) return nullptr;

		return &*Iterator;
	}

	void Run( const Config& Config, std::ofstream& File )
	{
		if ( !is_directory( Config.Downloads ) || !is_directory( Config.Target ) ) { return; }

		auto CreateDirectory = [&Config] ( const std::string_view DirectoryName )
		{
			const auto Path = Config.Target / DirectoryName / "";
			std::filesystem::create_directory( Path );
			return Path;
		};

		const std::string           MiscellaneousName = "XX - Miscellaneous";
		const std::filesystem::path MiscellaneousPath = CreateDirectory( MiscellaneousName );

		RuntimeRules.reserve( Rules.size() );

		for ( const auto& Rule : Rules )
		{
			const std::string     FormattedName = Rule.GetFormattedDirectoryName();
			std::filesystem::path Path          = CreateDirectory( FormattedName );
			RuntimeRules.emplace_back( Rule, Path );
		}

		const size_t AmountOfEntries = std::distance( std::filesystem::directory_iterator( Config.Downloads ), std::filesystem::directory_iterator() );

		bool IsFirstProgressLine = true;
		bool DidPrintAnything    = false;

		for ( const auto&& [ Index, Entry ] : std::filesystem::directory_iterator( Config.Downloads ) | std::views::enumerate )
		{
			const RuntimeRule*           Matched    = GetType( Entry.path() );
			const std::filesystem::path* TargetPath = Matched ? &Matched->Path : &MiscellaneousPath;
			std::error_code              ErrorCode;
			std::filesystem::path        Destination = *TargetPath / Entry.path().filename();

			if ( std::filesystem::equivalent( Entry.path(), Destination, ErrorCode ) ) { continue; }

			if ( ErrorCode ) { ErrorCode.clear(); }

			File << std::format( "- Moving '{}' to {}\n", Entry.path().filename().string(), Matched ? Matched->Rule.DirectoryName : "Miscellaneous" );
			ShowProgress( Entry.path().filename().string(), TargetPath->string(), static_cast<float>( Index ) / static_cast<float>( AmountOfEntries ), IsFirstProgressLine );
			DidPrintAnything = true;

			if ( Config.IsDryRun ) continue;

			std::filesystem::rename( Entry.path(), Destination, ErrorCode );

			if ( ErrorCode == std::errc::cross_device_link )
			{
				std::filesystem::copy( Entry.path(), Destination, std::filesystem::copy_options::recursive, ErrorCode );

				if ( !ErrorCode ) { std::filesystem::remove_all( Entry.path(), ErrorCode ); }
			}

			if ( ErrorCode ) { File << std::format( "    ^- Failed with '{}'\n", ErrorCode.message() ); }
		}

		if ( DidPrintAnything ) std::print( "\n" );
	}
}

int main( const int argc, char** argv )
{
	if ( argc < 3 )
	{
		std::println( "[-] Improper Usage! Proper usage is:" );
		std::println( R"(  {} "<DownloadDirectory>" "<TargetDirectory>" [parameters])", argv[ 0 ] );
		std::println( "    DownloadDirectory: Directory that will be sorted (e.g. C:/Users/Blanche/Downloads)" );
		std::println( "    TargetDirectory: Directory that items will be moved to (e.g. E:/SortedDownloads)" );
		std::println( "    Parameters (optional): " );
		std::println( "      --dryrun: Only outputs where items would be moved; does not actually move items." );
		std::println( "      --reorganize: Iterates through directories in <TargetDirectory>, useful if you changed rules!" );
		return 0;
	}

	Config Current;
	Current.Downloads = argv[ 1 ];
	Current.Target    = argv[ 2 ];

	for ( int i = 3; i < argc; ++i )
	{
		const std::string_view Argument = argv[ i ];
		Current.IsDryRun                = Current.IsDryRun || Argument == "--dryrun";
		Current.IsReOrganize            = Current.IsReOrganize || Argument == "--reorganize";
	}

	auto Now       = std::chrono::floor<std::chrono::seconds>( std::chrono::system_clock::now() );
	auto ZonedTime = std::chrono::zoned_time( std::chrono::current_zone(), Now );

	std::println( "Organize Downloads started @ {:%T %Z on %A, %B %d, %Y}", ZonedTime );

	if ( Current.IsDryRun ) { std::println( "[#] Dry Run mode is on! Nothing will actually be moved; you will simply get output as if." ); }

	const std::filesystem::path LogPath = Current.Target / "OrganizeLogs.txt";
	std::ofstream               File( LogPath, std::ios::app );

	if ( !File.is_open() )
	{
		std::println( "[!] Failed to open log file." );
		return -1;
	}

	File << std::format( "{:=<100}\n", "" );
	File << std::format( "[Organize Downloads started @ {:%T %Z} on {:%A, %B %d, %Y}]\n", ZonedTime, ZonedTime );
	File << std::format( "  Organizing '{}' to '{}...'\n", Current.Downloads.string(), Current.Target.string() );
	File << std::format( "  Parameters: Dry run? {}\tRe-Organize? {}\n", Current.IsDryRun, Current.IsReOrganize );
	File << std::format( "{:=<100}\n", "" );

	if ( Current.IsReOrganize )
	{
		std::filesystem::path OriginalDownloadFolder = Current.Downloads;

		for ( const auto& Entry : std::filesystem::directory_iterator( Current.Target ) )
		{
			Current.Downloads = Entry.path();
			Run( Current, File );
		}

		Current.Downloads = OriginalDownloadFolder;
	}

	Run( Current, File );

	Now       = std::chrono::floor<std::chrono::seconds>( std::chrono::system_clock::now() );
	ZonedTime = std::chrono::zoned_time( std::chrono::current_zone(), Now );

	File << std::format( "{:=<100}\n", "" );
	File.close();
}
