#pragma once
#include <algorithm>
#include <array>
#include <filesystem>

namespace Images
{
	using std::string_view_literals::operator ""sv;

	static constexpr std::array HardMatchExtensions =
	{
		".jpg"sv,
		".jpeg"sv,
		".jpe"sv,
		".jfif"sv,
		".png"sv,
		".gif"sv,
		".bmp"sv,
		".dib"sv,
		".tif"sv,
		".tiff"sv,
		".webp"sv,

		".avif"sv,
		".jxl"sv,
		".heic"sv,
		".heif"sv,

		".svg"sv,
		".svgz"sv,

		".ico"sv,

		".raw"sv,
		".cr2"sv,
		".cr3"sv,
		".crw"sv,
		".nef"sv,
		".nrw"sv,
		".arw"sv,
		".srf"sv,
		".sr2"sv,
		".orf"sv,
		".rw2"sv,
		".raf"sv,
		".pef"sv,
		".dng"sv,
		".3fr"sv,
		".iiq"sv,
		".kdc"sv,
		".dcr"sv,
		".erf"sv,
		".mrw"sv,
		".x3f"sv,

		".psd"sv,
		".psb"sv,
		".xcf"sv,
		".tga"sv,
		".pcx"sv,
		".ppm"sv,
		".pgm"sv,
		".pbm"sv,
		".pnm"sv,
		".pam"sv,
		".hdr"sv,
		".exr"sv
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
