#pragma once

namespace process::identity
{
	enum class ByondIdentity
	{
		kByond,
		kDreamSeeker,
		kDreamDaemon,
		kDreamMaker,
		kUnknown
	};

	ByondIdentity getByondIdentity();
}
