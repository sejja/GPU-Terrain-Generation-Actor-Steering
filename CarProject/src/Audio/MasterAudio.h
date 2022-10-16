//
//  MasterAudio.h
//  BaleaEngine
//
//  Created by Diego Revilla on 28/3/20.
//  Copyright � 2020 . All rights reserved.
//

#ifndef _MASTER_AUDIO__H_
#define _MASTER_AUDIO__H_

namespace FMOD {
	class Sound;
	class Channel;
	class System;
}

namespace BaleaEngine {
	namespace Audio {
		struct Sound {
			Sound();

			FMOD::Sound* pSound;
			std::string filename;
		};

		void SetMasterVolume(const float f);
		const float GetMasterVolume(void);
		class SoundGroup;

		class Voice {
			// friends have access to privates
			friend class AudioManager;
			friend class SoundGroup;

		public:

			Voice();
			~Voice();

			// Setters and Getters
			void SetVolume(float volume);
			void SetPause(bool paused);
			void SetLoop(bool isLoop);
			bool IsPaused();
			bool IsValid();
			float GetVolume();
			void SetPitch(float pitch);
			float GetPitch();
			bool IsLooping() const;
			FMOD::Channel* GetChannel() { return pChannel; }

		private:
			FMOD::Channel* pChannel;
			SoundGroup* sgroup = nullptr;
			float volume = 1.f;
			bool mLoop = false;
		};

		class SoundGroup {
		public:
			void SetGroupVolume(float volume);
			float GetGroupVolume() const;
			void SetGroupPitch(float pitch);
			float GetGroupPitch();
			void AddVoice(Voice& voice);
			void RemoveVoice(Voice& voice);

		private:
			std::list<Voice*> voices;
			float groupVolume = 1.f;
			float groupPitch = 1.f;
		};
	}
}

#endif
