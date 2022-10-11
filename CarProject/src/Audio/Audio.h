//
//  Audio.h
//  BaleaEngine
//
//  Created by Diego Revilla on 28/3/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#ifndef _AUDIO__H_
#define _AUDIO__H_

namespace BaleaEngine {
	namespace Audio {
		class AudioManager {
		public:

			bool Initialize();
			Sound* CreateSound(const char* filename);
			STATIC FreeSound(Sound* pSound);
			STATIC StopAll();
			STATIC FreeVoice(Voice* pVoice);
			STATIC Play(const Sound* pSound, bool paused, Voice& outvoice);
			STATIC Loop(Voice* pSound);
			DONTDISCARD size_t GetFreeVoiceCount() const;
			DONTDISCARD size_t GetUsedVoiceCount() const;
			DONTDISCARD size_t GetTotalVoiceCount() const;
			STATIC Update();
			static AudioManager& GetInstance();
			STATIC SetMasterVolume(float f);
			float GetMasterVolume() const;
			SoundGroup& GetSoundGroup(const std::string& str);

		protected:
			AudioManager();
			~AudioManager();
			STATIC AllocateVoices();
			STATIC FreeVoices();
			DONTDISCARD Voice* GetFreeVoice();
			FMOD::System* pFMOD;
			size_t soundCount;
			size_t voiceCount;
			Voice* voices;
			std::list<Voice*> freeVoices;
			std::list<Voice*> usedVoices;
			std::map<std::string, SoundGroup> soundgroups;

		private:
			float mastervol = 1;
		};

		class AudioEmitter : public Component {
		public:
			void Initialize() override;
			void OnGui() override {}
			void FromJson(const nlohmann::json& val) noexcept override {}
			void ToJson(nlohmann::json& val) const noexcept override {}
			~AudioEmitter();
			std::string audioFile;

		private:
			Voice voice;
			Sound* sound;
			bool mLoop;
		};

		class Audio3DEmitter : public Component {
		public:
			void Initialize() override;
			void Update() override;
			void OnGui() override {}
			void FromJson(const nlohmann::json& val) noexcept override {}
			void ToJson(nlohmann::json& val) const noexcept override {}
			~Audio3DEmitter();

		private:
			Voice voice;
			Sound* sound;
			std::string audioFile;
			float customLevel = 1.f;
			float centerFrequency = 0.f;
			float dopplerEffect = 0.f;
			float maxDistance = 0.f;
			float minDistance = 0.f;
			bool mLoop;
		};
	}
}

#define AAudio (&BaleaEngine::Audio::AudioManager::GetInstance())

#endif