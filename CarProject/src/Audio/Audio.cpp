//
//  Audio.cpp
//  BaleaEngine
//
//  Created by Diego Revilla on 28/3/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#include <Shared.h>

#define MAX_VOICE_COUNT	100

namespace BaleaEngine {
	namespace Audio {
#pragma region// Audio Manager

		static stl::PageAllocator<Sound> soundalloc;

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Initializes the Audio Manager
		*/ // --------------------------------------------------------------------
		AudioManager::AudioManager() :
			pFMOD(NULL), soundCount(0), voiceCount(MAX_VOICE_COUNT), voices(NULL) {
		}

		// ------------------------------------------------------------------------
		/*! Default Destructor
		*
		*   Destroys the Audio Manager
		*/ // --------------------------------------------------------------------
		AudioManager::~AudioManager() {
			//if we did have fmod initialized
			if (pFMOD) {
				pFMOD->release();
				FreeVoices();
			}
		}

		// ------------------------------------------------------------------------
		/*! Get Instance
		*
		*   Gets the instance of the audio manager
		*/ // -------------------------------------------------------------------
		AudioManager& AudioManager::GetInstance() {
			static AudioManager audmgr_;

			return audmgr_;
		}

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   Initializes the Audio Manager
		*/ // --------------------------------------------------------------------
		bool AudioManager::Initialize() {
			FMOD_RESULT result = FMOD::System_Create(&pFMOD);

			//if we were not able to initialize FMOD
			if (result)
				return false;

			// Initialize FMOD
			result = pFMOD->init(MAX_VOICE_COUNT, FMOD_INIT_NORMAL, 0);

			//if we could not initialize all out voices
			if (result) {
				pFMOD->release();
				pFMOD = nullptr;

				return false;
			}

			AllocateVoices();

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Audio Manager
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::Update() {
			//if we do have an FMOD system created
			if (pFMOD) {
				pFMOD->update();

				// loop through the voices
				for (std::list<Voice*>::iterator it = usedVoices.begin(),
					endx = usedVoices.end(); it != endx; ++it) {
					// this voice is no longer playing
					if (!(*it)->IsValid()) {
						freeVoices.push_back(*it);
						it = usedVoices.erase(it);

						//if we are at the end of our voice list
						if (it == endx)
							break;
					}
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Free Voice
		*
		*   Free�s a voice
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::FreeVoice(Voice* pVoice) {
			//If we do have an Audio Manager and a voice
			if (pFMOD && pVoice) {
				pVoice->pChannel->stop();
				usedVoices.remove(pVoice);
				freeVoices.push_back(pVoice);
			}
		}

		SoundGroup& AudioManager::GetSoundGroup(const std::string& str) {
			return soundgroups[str];
		}

		// ------------------------------------------------------------------------
		/*! Get Free Voice Count
		*
		*   Returns how many free voices we do have
		*/ // --------------------------------------------------------------------
		DONTDISCARD size_t AudioManager::GetFreeVoiceCount() const {
			return freeVoices.size();
		}

		// ------------------------------------------------------------------------
		/*! Get Used Voice Count
		*
		*   Returns how many voices have we used
		*/ // --------------------------------------------------------------------
		DONTDISCARD size_t AudioManager::GetUsedVoiceCount() const {
			return usedVoices.size();
		}

		// ------------------------------------------------------------------------
		/*! Get Total Voice Count
		*
		*  Get the total voice count
		*/ // --------------------------------------------------------------------
		DONTDISCARD size_t AudioManager::GetTotalVoiceCount() const {
			return voiceCount;
		}

		// ------------------------------------------------------------------------
		/*! Get Free Voice
		*
		*   Returns a free voice
		*/ // --------------------------------------------------------------------
		DONTDISCARD Voice* AudioManager::GetFreeVoice() {
			//if we do have an FMOD system and the free voices list is not empty
			if (pFMOD && !freeVoices.empty()) {
				Voice* pv = freeVoices.back();
				freeVoices.pop_back();
				usedVoices.push_back(pv);

				return pv;
			}

			return NULL;
		}

		// ------------------------------------------------------------------------
		/*! Allocate Voices
		*
		*   Allocates the Voices that we might need
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::AllocateVoices() {
			stl::allocator<Voice> alloc;

			//if we do have an FMOD System
			if (pFMOD) {
				FreeVoices();
				voiceCount = MAX_VOICE_COUNT;
				voices = alloc.allocate(voiceCount);

				for (decltype(voiceCount) i = 0; i < voiceCount; i++)
					alloc.construct(voices + i);

				stl::FForEach(voices, voices + voiceCount, [this](Voice& v) {
					freeVoices.push_back(&v);
					});
			}
		}

		// ------------------------------------------------------------------------
		/*! Free Voices
		*
		*   Free all the voices within the Audio Manager
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::FreeVoices() {
			stl::allocator<Voice> alloc;

			//if we do have an FMOD system and some voices
			if (pFMOD && voices) {

				for (size_t i = 0; i < voiceCount; i++) alloc.destroy(voices + i);
				alloc.deallocate(voices);
				voiceCount = 0;
				freeVoices.clear();
				usedVoices.clear();
			}
		}

		// ------------------------------------------------------------------------
		/*! Create Sound
		*
		*   Creates a sound from a file
		*/ // --------------------------------------------------------------------
		Sound* AudioManager::CreateSound(const char* filename) {
			if (pFMOD) {
				Sound* pSound = soundalloc.allocate();
				soundalloc.construct(pSound);
				pFMOD->createSound(filename,
					FMOD_LOOP_NORMAL | FMOD_3D, 0, &pSound->pSound);
				pSound->filename = filename;

				// error check
				if (!pSound->pSound) {
					soundalloc.destroy(pSound);
					soundalloc.deallocate(pSound);
					return nullptr;
				}

				++soundCount;

				return pSound;
			}

			return nullptr;
		}

		// ------------------------------------------------------------------------
		/*! Free Sound
		*
		*   Frees a sound from our Audio Manager
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::FreeSound(Sound* pSound) {
			//if we do have a FMOD system and the given sound is valid
			if (pFMOD && pSound) {
				//if we had a sound
				if (pSound->pSound) {
					pSound->pSound->release();
					pSound->pSound = 0;
				}

				--soundCount;
			}
		}

		// ------------------------------------------------------------------------
		/*! Play
		*
		*   Plays a certain sound
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::Play(const Sound* pSound, const bool paused, Voice& outvoice) {
			// make sure we can actually play the sound
			if (pFMOD && pSound) {
				Voice* pVoice = GetFreeVoice();

				// if this new voice is valid
				if (pVoice) {
					pFMOD->playSound(pSound->pSound, 0, paused, &pVoice->pChannel);
					pVoice->SetPause(paused);
					pVoice->SetLoop(false);
					outvoice = *pVoice;
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Loop
		*
		*   Loops a Voice
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::Loop(Voice* pVoice) {
			if (pVoice)
				pVoice->SetLoop(true);
		}

		// ------------------------------------------------------------------------
		/*! Play
		*
		*   Plays a certain sound
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::StopAll() {
			//if we do have an FMOD system
			if (pFMOD) {
				// loop through the voices
				while (!usedVoices.empty()) {
					usedVoices.back()->pChannel->stop();
					freeVoices.push_back(usedVoices.back());
					usedVoices.pop_back();
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Set Master Volume
		*
		*   Sets the maximun value
		*/ // --------------------------------------------------------------------
		STATIC AudioManager::SetMasterVolume(const float f) {
			//If the volume is higher than allowed
			if (f > 1.f)
				mastervol = 1.f;
			//If the volume is lower than allowed
			else if (f < 0.f)
				mastervol = 0.f;
			else
				mastervol = f;

			stl::FForEach(usedVoices.begin(), usedVoices.end(),
				[this](Voice*& v) {v->SetVolume(v->GetVolume() * mastervol); });
		}

		// ------------------------------------------------------------------------
		/*! Get Master Volume
		*
		*   Gets the Master volume audio
		*/ // --------------------------------------------------------------------
		float AudioManager::GetMasterVolume() const {
			return mastervol;
		}

#pragma endregion

		void AudioEmitter::Initialize() {
			auto content = ResourceMgr->Load<Sound>(audioFile.c_str());
			sound = content->get();
			if (content && sound)
				AAudio->Play(sound, false, voice);
			voice.SetLoop(mLoop);
		}

		AudioEmitter::~AudioEmitter() {
			voice.SetPause(true);
		}

		void Audio3DEmitter::Initialize() {
			auto content = ResourceMgr->Load<Sound>(audioFile.c_str());
			sound = content->get();
			if (content && sound)
				AAudio->Play(sound, false, voice);
			voice.SetLoop(mLoop);
			voice.SetVolume(customLevel);
		}

		void Audio3DEmitter::Update() {
			auto position = GetOwner()->GetPosition(), campos =
				reinterpret_cast<EditorCamera*>(Editor->GetCurrentScene()->mViewCamera)->
				GetOwner()->GetPosition();
			FMOD_VECTOR vec;
			const float distancefactor = (1.f / (maxDistance - minDistance));
			vec.x = (position.x - campos.x) * distancefactor;
			vec.y = (position.y - campos.y) * distancefactor;
			vec.z = (position.z - campos.z) * distancefactor;

			voice.GetChannel()->set3DAttributes(&vec, nullptr);
			voice.GetChannel()->set3DDopplerLevel(dopplerEffect);
			voice.GetChannel()->set3DDistanceFilter(true, customLevel, centerFrequency);
		}

		Audio3DEmitter::~Audio3DEmitter() {
			voice.SetPause(true);
		}
	}
}
