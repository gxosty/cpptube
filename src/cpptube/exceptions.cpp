#include <cpptube/exceptions.hpp>

namespace cpptube::exceptions
{
	const char* CpptubeError::what()
	{
		return this->error_msg.c_str();
	}

	RegexMatchError::RegexMatchError(const std::string& caller, const std::string& pattern)
	{

	}

	VideoUnavailableError::VideoUnavailableError(const std::string& video_id) : video_id{video_id}
	{
		this->error_msg = video_id + " is unavailable";
	}

	AgeRestrictedError::AgeRestrictedError(const std::string& video_id)
	{
		this->video_id = video_id;
		this->error_msg = this->video_id + " is age restricted, and can't be accessed without logging in";
	}

	LiveStreamError::LiveStreamError(const std::string& video_id)
	{
		this->video_id = video_id;
		this->error_msg = this->video_id + " is streaming live and cannot be loaded";
	}

	VideoPrivateError::VideoPrivateError(const std::string& video_id)
	{
		this->video_id = video_id;
		this->error_msg = this->video_id + " is a private video";
	}

	RecordingUnavailableError::RecordingUnavailableError(const std::string& video_id)
	{
		this->video_id = video_id;
		this->error_msg = this->video_id + " does not have a live stream recording available";
	}

	MembersOnlyError::MembersOnlyError(const std::string& video_id)
	{
		this->video_id = video_id;
		this->error_msg = this->video_id + " is a members-only video";
	}

	VideoRegionBlockedError::VideoRegionBlockedError(const std::string& video_id)
	{
		this->video_id = video_id;
		this->error_msg = this->video_id + " is not available in your region";
	}
}