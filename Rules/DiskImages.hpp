#pragma once
#include <algorithm>
#include <filesystem>

namespace DiskImages
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".iso"sv,
		".img"sv,
		".ima"sv,

		".vhd"sv,
		".vhdx"sv,
		".vmdk"sv,
		".vdi"sv,
		".vhd"sv,
		".hdd"sv,
		".qcow"sv,
		".qcow2"sv,
		".qed"sv,

		".dmg"sv,
		".sparseimage"sv,
		".sparsebundle"sv,

		".wim"sv,
		".esd"sv,
		".swm"sv,

		".vfd"sv,
		".flp"sv,

		".bin"sv,
		".cue"sv,
		".mdf"sv,
		".mds"sv,
		".ccd"sv,
		".sub"sv,
		".nrg"sv,

		".vmdk"sv,
		".ova"sv,
		".ovf"sv,

		".hfs"sv,
		".hfsplus"sv,
		".dsk"sv,
		".d64"sv,
		".d71"sv,
		".d81"sv,
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
