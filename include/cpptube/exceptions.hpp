#pragma once

#include <string>
#include <stdexcept>

namespace cpptube::exceptions
{
	class CpptubeError : public std::exception
	{
	protected:
		std::string error_msg = "An error occured";
	public:
		CpptubeError() = default;
		CpptubeError(const std::string& error_msg);

		const char* what();
	};

	class MaxRetriesExceededError : public CpptubeError {};

	class HTMLParseError : public CpptubeError
	{
	public:
		HTMLParseError() = default;
		HTMLParseError(const std::string& error_msg);
	};

	class ExtractError : public CpptubeError {};

	class RegexMatchError : public ExtractError
	{
	public:
		std::string caller;
		std::string pattern;
		// passing pattern as :std::string: because there is no way to retrieve
		// original pattern as :std::string: from :std::regex: object
		RegexMatchError(const std::string& caller, const std::string& pattern);
	};

	class VideoUnavailableError : public CpptubeError
	{
	public:
		std::string video_id;
		VideoUnavailableError();
		VideoUnavailableError(const std::string& video_id);
	};

	class AgeRestrictedError : public VideoUnavailableError
	{
	public:
		AgeRestrictedError(const std::string& video_id);
	};

	class LiveStreamError : public VideoUnavailableError
	{
	public:
		LiveStreamError(const std::string& video_id);
	};

	class VideoPrivateError : public VideoUnavailableError
	{
	public:
		VideoPrivateError(const std::string& video_id);
	};

	class RecordingUnavailableError : public VideoUnavailableError
	{
	public:
		RecordingUnavailableError(const std::string& video_id);
	};

	class MembersOnlyError : public VideoUnavailableError
	{
	public:
		MembersOnlyError(const std::string& video_id);
	};

	class VideoRegionBlockedError : public VideoUnavailableError
	{
	public:
		VideoRegionBlockedError(const std::string& video_id);
	};
}