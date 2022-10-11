//
//  MasterAudio.cpp
//  BaleaEngine
//
//  Created by Diego Revilla on 28/3/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Audio {
#pragma region// Voice

		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   The constructor for the Voice
		*/ // ---------------------------------------------------------------------
		Voice::Voice()
			: pChannel(NULL) {
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Destroys a Voice
		*/ // --------------------------------------------------------------------
		Voice::~Voice() {
			//If we are in a group
			if (sgroup)
				sgroup->RemoveVoice(*this);
		}

		// ------------------------------------------------------------------------
		/*! Set Volume
		*
		*   Sets the volume of a channel
		*/ // --------------------------------------------------------------------
		void Voice::SetVolume(float vol) {
			volume = vol;

			//if the Channel exists
			if (pChannel)
				//if the volume is less than 0
				if (vol < 0)
					pChannel->setVolume(0.f);

			//if the volume is higher than 0
				else if (vol > 1.f)
					pChannel->setVolume(1.f);
				else
					pChannel->setVolume(volume);
		}

		// ------------------------------------------------------------------------
		/*! Get Volume
		*
		*   Gets the volume of a channel
		*/ // --------------------------------------------------------------------
		DONTDISCARD float Voice::GetVolume() {
			return volume;
		}

		// ------------------------------------------------------------------------
		/*! Set Pause
		*
		*   Sets a channel to pause
		*/ // --------------------------------------------------------------------
		void Voice::SetPause(bool paused) {
			//if we have a valid channel
			if (pChannel)
				pChannel->setPaused(paused);
		}

		// ------------------------------------------------------------------------
		/*! Is Paused
		*
		*   Returns whether a voice is paused or not
		*/ // --------------------------------------------------------------------
		DONTDISCARD bool Voice::IsPaused() {
			//if we have a valid channel
			if (pChannel) {
				bool _paused;

				pChannel->getPaused(&_paused);

				return _paused;
			}

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Set Loop
		*
		*   Sets a channel to loop or stops looping
		*/ // --------------------------------------------------------------------
		void  Voice::SetLoop(bool isLoop) {
			//if we have a valid channel
			if (pChannel) {
				//if we are setting it to loop
				if (isLoop) {
					pChannel->setLoopCount(-1);

					//if we are setting it to not loop
				}
				else {
					pChannel->setLoopCount(0);
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Is Looping
		*
		*   Returns whether a voice is looping or not
		*/ // --------------------------------------------------------------------
		DONTDISCARD bool Voice::IsLooping() const {
			//if we have a valid channel
			if (pChannel) {
				int _loop;

				pChannel->getLoopCount(&_loop);

				//if we are actually looping
				if (_loop) {
					return true;
				}

				return false;
			}

			return false;
		}

		// ------------------------------------------------------------------------
		/*! Set Pitch
		*
		*   Sets the pitch of a channel
		*/ // --------------------------------------------------------------------
		DONTDISCARD void Voice::SetPitch(float pitch) {
			//if we have a valid channel
			if (pChannel) {
				pChannel->setPitch(pitch);
			}
		}

		// ------------------------------------------------------------------------
		/*! Get Pitch
		*
		*   Returns whether a channel has a certain pitch or not
		*/ // --------------------------------------------------------------------
		DONTDISCARD float Voice::GetPitch() {
			//if we have a valid channel
			if (pChannel) {
				float _pitch;

				pChannel->getPitch(&_pitch);

				return _pitch;
			}

			return 0;
		}

		// ------------------------------------------------------------------------
		/*! Is Valid
		*
		*   Returns whether a voice is valid or not (Valid = Playing and existing,
		*		Not Valid = Not existing or not playing)
		*/ // --------------------------------------------------------------------
		DONTDISCARD bool Voice::IsValid() {
			if (pChannel) {
				bool pl;

				pChannel->isPlaying(&pl);

				return pl;
			}

			return false;
		}

#pragma endregion

		// ----------------------------------------------------------------------------
#pragma region// Sound

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Default Constructor for the Sound Class
		*/ // --------------------------------------------------------------------
		Sound::Sound()
			: pSound(NULL) {
		}

		// ------------------------------------------------------------------------
		/*! Set Group Volume
		*
		*   Sets the Volume of a group
		*/ // --------------------------------------------------------------------
		void SoundGroup::SetGroupVolume(float volume) {
			//If the volume is above allowed levels
			if (volume > 1.f)
				groupVolume = 1.f;

			//If the volume is below allowed levels
			else if (volume < 0.f)
				groupVolume = 0.f;
			else
				groupVolume = volume;

			stl::FForEach(voices.begin(), voices.end(),
				[this](Voice*& v) {v->SetVolume(v->GetVolume() * groupVolume); });
		}

		// ------------------------------------------------------------------------
		/*! Get Group Volume
		*
		*   Gets the Volume of a group
		*/ // --------------------------------------------------------------------
		float SoundGroup::GetGroupVolume() const {
			return groupVolume;
		}

		// ------------------------------------------------------------------------
		/*! Set Group Pitch
		*
		*   Sets the Pitch of a group
		*/ // --------------------------------------------------------------------
		void SoundGroup::SetGroupPitch(float pitch) {
			groupPitch = pitch;

			stl::FForEach(voices.begin(), voices.end(),
				[this](Voice*& v) {v->SetPitch(v->GetPitch() * groupVolume); });
		}

		// ------------------------------------------------------------------------
		/*! Get Group Pitch
		*
		*   Gets the Pitch of a group
		*/ // --------------------------------------------------------------------
		float SoundGroup::GetGroupPitch() {
			return groupPitch;
		}

		// ------------------------------------------------------------------------
		/*! Add Voice
		*
		*   Adds a voice to the soundgroup
		*/ // --------------------------------------------------------------------
		void SoundGroup::AddVoice(Voice& voice) {
			voices.push_back(&voice);
			voice.sgroup = this;
		}

		// ------------------------------------------------------------------------
		/*! Remove Voice
		*
		*   Removes a voice from this group
		*/ // --------------------------------------------------------------------
		void SoundGroup::RemoveVoice(Voice& voice) {
			voices.remove(&voice);
		}
	}
}

#pragma endregion
