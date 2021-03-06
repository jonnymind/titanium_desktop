/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _MEDIA_SOUND_H_
#define _MEDIA_SOUND_H_
#include <kroll/kroll.h>

using namespace kroll;

namespace ti
{
	class Sound : public StaticBoundObject
	{
		public:
		Sound(std::string& url);
		virtual ~Sound();

		void Play(const ValueList& args, SharedValue result);
		void Pause(const ValueList& args, SharedValue result);
		void Stop(const ValueList& args, SharedValue result);
		void Reload(const ValueList& args, SharedValue result);
		void SetVolume(const ValueList& args, SharedValue result);
		void GetVolume(const ValueList& args, SharedValue result);
		void SetLooping(const ValueList& args, SharedValue result);
		void IsLooping(const ValueList& args, SharedValue result);
		void IsPlaying(const ValueList& args, SharedValue result);
		void IsPaused(const ValueList& args, SharedValue result);
		void SetOnComplete(const ValueList& args, SharedValue result);

		void Play();
		void Pause();
		void Stop();
		void Reload();
		void SetVolume(double newVolume);
		double GetVolume();
		void SetLooping(bool newLooping);
		bool IsLooping();
		bool IsPlaying();
		bool IsPaused();
		void SetOnComplete(SharedKMethod newCallback);
		void Load();
		void Unload();
		void SoundCompletedIteration();

		virtual void LoadImpl() = 0;
		virtual void UnloadImpl() = 0;
		virtual void PlayImpl() = 0;
		virtual void PauseImpl() = 0;
		virtual void StopImpl() = 0;
		virtual void SetVolumeImpl(double volume) = 0;
		virtual double GetVolumeImpl() = 0;;

		protected:
		enum SoundState { PLAYING, PAUSED, STOPPED, END_OF_ITERATION };
		SoundState state;
		std::string url;
		SharedKMethod callback;
		bool looping;
	};
}

#endif
