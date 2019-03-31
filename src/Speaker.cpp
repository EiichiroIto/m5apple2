/*
AppleWin : An Apple //e emulator for Windows

Copyright (C) 1994-1996, Michael O'Brien
Copyright (C) 1999-2001, Oliver Schmidt
Copyright (C) 2002-2005, Tom Charlesworth
Copyright (C) 2006-2007, Tom Charlesworth, Michael Pohoreski

AppleWin is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

AppleWin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AppleWin; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* Description: Speaker emulation
 *
 * Author: Various
 */

/* Remake for SDL Audio for Linux (or other SDL-compliant OSes) by beom beotiger --bb */

#include "stdafx.h"
// for _ASSERT ion. (here _ASSERT means Unix(tm) assert) --bb
#include <assert.h>

// Notes:
//
// [OLD: 23.191 Apple CLKs == 44100Hz (CLK_6502/44100)]
// 23 Apple CLKS per PC sample (played back at 44.1KHz)
//
//
// The speaker's wave output drives how much 6502 emulation is done in real-time, eg:
// If the speaker's wave buffer is running out of sample-data, then more 6502 cycles
// need to be executed to top-up the wave buffer.
// This is in contrast to the AY8910 voices, which can simply generate more data if
// their buffers are running low.


// number of channels and buffer size for Apple][ Speakers
static const unsigned short g_nSPKR_NumChannels = 1;
//-------------------------------------

// Globals (SOUND_WAVE)
const short		SPKR_DATA_INIT = (short)0x8000;	// data written to speakers buffer

static short	g_nSpeakerData	= SPKR_DATA_INIT;
static short	*g_pSpeakerBuffer = NULL;
static UINT	g_nBufferIdx	= 0;

static short	*g_pStereoBuffer = NULL;	// buffer for stereo samples


static short	*g_pRemainderBuffer = NULL;	// Remainder buffer
static UINT	g_nRemainderBufferSize;		// Setup in SpkrInitialize()
static UINT	g_nRemainderBufferIdx;		// Setup in SpkrInitialize()


// Application-wide globals:
DWORD		soundtype		= SOUND_WAVE; //default
double		    g_fClksPerSpkrSample;		// Setup in SetClksPerSpkrSample()

// Globals
static unsigned __int64	g_nSpkrQuietCycleCount = 0;
static unsigned __int64 g_nSpkrLastCycle = 0;
static bool g_bSpkrToggleFlag = false;

static bool g_bSpkrAvailable = false;
static bool g_bSpkrRecentlyActive = false;

//-----------------------------------------------------------------------------

// Forward refs:
//ULONG   Spkr_SubmitWaveBuffer_FullSpeed(short* pSpeakerBuffer, ULONG nNumSamples);
static ULONG   Spkr_SubmitWaveBuffer(short* pSpeakerBuffer, ULONG nNumSamples);
static void    Spkr_SetActive(bool bActive);

//=============================================================================
// Let us leave benchmark for the near future --bb ^_^
#if 0
static void DisplayBenchmarkResults ()
{

  DWORD totaltime = GetTickCount() - extbench;
  VideoRedrawScreen();
  TCHAR buffer[64];
  sprintf(buffer,
           TEXT("This benchmark took %u.%02u seconds."),
           (unsigned)(totaltime / 1000),
           (unsigned)((totaltime / 10) % 100));
/*  MessageBox(g_hFrameWindow,
             buffer,
             TEXT("Benchmark Results"),
             MB_ICONINFORMATION | MB_SETFOREGROUND);*/
  printf("This benchmark took %u.%02u seconds.",
	 	(unsigned)(totaltime / 1000), (unsigned)((totaltime / 10) % 100));

}
#endif

//=============================================================================

static void SetClksPerSpkrSample()
{
//	// 23.191 clks for 44.1Khz (when 6502 CLK=1.0Mhz)
//	g_fClksPerSpkrSample = g_fCurrentCLK6502 / (double)SPKR_SAMPLE_RATE;

	// Use integer value: Better for MJ Mahon's RT.SYNTH.DSK (integer multiples of 1.023MHz Clk)
	// . 23 clks @ 1.023MHz		SPKR_SAMPLE_RATE = 44100Hz!?
	g_fClksPerSpkrSample = (double) (UINT) (g_fCurrentCLK6502 / (double)SPKR_SAMPLE_RATE);
}

//=============================================================================

static void InitRemainderBuffer()
{
	delete [] g_pRemainderBuffer;

	SetClksPerSpkrSample();

	g_nRemainderBufferSize = (UINT) g_fClksPerSpkrSample;
	if ((double)g_nRemainderBufferSize != g_fClksPerSpkrSample)
		g_nRemainderBufferSize++;

	g_pRemainderBuffer = new short [g_nRemainderBufferSize];
	memset(g_pRemainderBuffer, 0, g_nRemainderBufferSize);

	g_nRemainderBufferIdx = 0;
}

//
// ----- ALL GLOBALLY ACCESSIBLE FUNCTIONS ARE BELOW THIS LINE -----
//
//=============================================================================

void SpkrDestroy ()
{
	Spkr_DSUninit();
	if(soundtype == SOUND_WAVE)
	{
		delete [] g_pSpeakerBuffer;
		delete [] g_pStereoBuffer;
		delete [] g_pRemainderBuffer;

		g_pSpeakerBuffer   = NULL;
		g_pStereoBuffer    = NULL;
		g_pRemainderBuffer = NULL;
	}
}

//=============================================================================

void SpkrInitialize ()
{
	if(g_fh)
	{
		fprintf(g_fh, "Spkr Config: soundtype = %d ", (int)soundtype);
		switch(soundtype)
		{
			case SOUND_NONE:   fprintf(g_fh, "(NONE)\n"); break;
			case SOUND_WAVE:   fprintf(g_fh, "(WAVE)\n"); break;
			default:           fprintf(g_fh, "(UNDEFINED!)\n"); break;
		}
	}

	if(g_bDisableDirectSound)
	{
//		SpeakerVoice.bMute = true;
	}
	else
	{
		//DSInit();
		g_bSpkrAvailable = Spkr_DSInit();
	}

	if (soundtype == SOUND_WAVE)
	{
		InitRemainderBuffer();
	   // Buffer can hold a max of 1 seconds worth of samples
#ifdef m5stack
		g_pSpeakerBuffer = (short*) VirtualAlloc(NULL, SPKR_SAMPLE_RATE * sizeof (short), 0,0);
		g_pStereoBuffer  = (short*) VirtualAlloc(NULL, SPKR_SAMPLE_RATE * 2 * sizeof (short), 0,0);
#else /* m5stack */
		g_pSpeakerBuffer = new short [SPKR_SAMPLE_RATE];
		g_pStereoBuffer  = new short [SPKR_SAMPLE_RATE * 2]; // doubled for stereo
#endif /* m5stack */
	}
}

//=============================================================================
// NB. Called when /g_fCurrentCLK6502/ changes
void SpkrReinitialize ()
{
	if (soundtype == SOUND_WAVE)
	{
		InitRemainderBuffer();
	}
}

//=============================================================================

void SpkrReset()
{
	g_nBufferIdx = 0;
	g_nSpkrQuietCycleCount = 0;
	g_bSpkrToggleFlag = false;

	InitRemainderBuffer();
	Spkr_SubmitWaveBuffer(NULL, 0);
	Spkr_SetActive(false);
	Spkr_Demute();
}

//=================2012 AD =========================================================
#if 0
BOOL SpkrSetEmulationType (DWORD newtype)
{
  if (soundtype != SOUND_NONE)
    SpkrDestroy();
  soundtype = newtype;
  if (soundtype != SOUND_NONE)
    SpkrInitialize();
  if (soundtype != newtype)
    switch (newtype) {	// some fault occured
      case SOUND_WAVE:
/*        MessageBox(window,
                   TEXT("The emulator is unable to initialize a waveform ")
                   TEXT("output device.  Make sure you have a sound card ")
                   TEXT("and a driver installed and that windows is ")
                   TEXT("correctly configured to use the driver.  Also ")
                   TEXT("ensure that no other program is currently using ")
                   TEXT("the device."),
                   TEXT("Configuration"),
                   MB_ICONEXCLAMATION | MB_SETFOREGROUND);*/
	      // Need to tuck SDL_GetError() hhere? ------------------------------------------
	      fprintf(stderr, "Unable to initialize a waveform output device.\n");
        return 0;

    }
  return 1;
}
#endif
//=============================================================================

static void ReinitRemainderBuffer(UINT nCyclesRemaining)
{
	if(nCyclesRemaining == 0)
		return;

	for(g_nRemainderBufferIdx=0; g_nRemainderBufferIdx<nCyclesRemaining; g_nRemainderBufferIdx++)
		g_pRemainderBuffer[g_nRemainderBufferIdx] = g_nSpeakerData;

	_ASSERT(g_nRemainderBufferIdx < g_nRemainderBufferSize);
}

static void UpdateRemainderBuffer(ULONG* pnCycleDiff)
{
	if(g_nRemainderBufferIdx)
	{
		while((g_nRemainderBufferIdx < g_nRemainderBufferSize) && *pnCycleDiff)
		{
			g_pRemainderBuffer[g_nRemainderBufferIdx] = g_nSpeakerData;
			g_nRemainderBufferIdx++;
			(*pnCycleDiff)--;
		}

		if(g_nRemainderBufferIdx == g_nRemainderBufferSize)
		{
			g_nRemainderBufferIdx = 0;
			signed long nSampleMean = 0;
			for(UINT i=0; i<g_nRemainderBufferSize; i++)
				nSampleMean += (signed long) g_pRemainderBuffer[i];
			nSampleMean /= (signed long) g_nRemainderBufferSize;

			if(g_nBufferIdx < SPKR_SAMPLE_RATE-1)
				g_pSpeakerBuffer[g_nBufferIdx++] = (short) nSampleMean;
		}
	}
}


static void UpdateSpkr()
{
  if(!g_bFullSpeed /*|| SoundCore_GetTimerState()*/)
  {
	  ULONG nCycleDiff = (ULONG) (g_nCumulativeCycles - g_nSpkrLastCycle);

	  UpdateRemainderBuffer(&nCycleDiff);

	  ULONG nNumSamples = (ULONG) ((double)nCycleDiff / g_fClksPerSpkrSample);

	  ULONG nCyclesRemaining = (ULONG) ((double)nCycleDiff - (double)nNumSamples * g_fClksPerSpkrSample);

	  while((nNumSamples--) && (g_nBufferIdx < SPKR_SAMPLE_RATE-1))
		  g_pSpeakerBuffer[g_nBufferIdx++] = g_nSpeakerData;

	  ReinitRemainderBuffer(nCyclesRemaining);	// Partially fill 1Mhz sample buffer
  }

  g_nSpkrLastCycle = g_nCumulativeCycles;
}

//=============================================================================

// Called by emulation code when Speaker I/O reg is accessed
BYTE SpkrToggle (WORD, WORD, BYTE, BYTE, ULONG nCyclesLeft)
{
  g_bSpkrToggleFlag = true;

  if(!g_bFullSpeed)
	Spkr_SetActive(true);

  needsprecision = cumulativecycles;	// ?

//   if (extbench)
//   {
//     DisplayBenchmarkResults();
//     extbench = 0;
//   }

  if (soundtype == SOUND_WAVE)
  {
	  CpuCalcCycles(nCyclesLeft);

	  UpdateSpkr();

	  g_nSpeakerData = ~g_nSpeakerData;
  }

  return MemReadFloatingBus(nCyclesLeft); // reading from $C030..$C03F retrurns unpredictable value?
}

//=============================================================================

// Called by ContinueExecution()
void SpkrUpdate (DWORD totalcycles)
{
  if(!g_bSpkrToggleFlag)
  {
	  if(!g_nSpkrQuietCycleCount)
	  {
		  g_nSpkrQuietCycleCount = g_nCumulativeCycles;
	  }
	  else if(g_nCumulativeCycles - g_nSpkrQuietCycleCount > (unsigned __int64)g_fCurrentCLK6502/5)
	  {
		  // After 0.2 sec of Apple time, deactivate spkr voice
		  // . This allows emulator to auto-switch to full-speed g_nAppMode for fast disk access
		  Spkr_SetActive(false);
	  }
  }
  else
  {
      g_nSpkrQuietCycleCount = 0;
      g_bSpkrToggleFlag = false;
  }

  if (soundtype == SOUND_WAVE)
  {
	  UpdateSpkr();
	  ULONG nSamplesUsed;

	  if(g_bFullSpeed) g_nBufferIdx = 0;	// try this --bb
//		  nSamplesUsed = Spkr_SubmitWaveBuffer/*_FullSpeed*/(g_pSpeakerBuffer, g_nBufferIdx);
	  else {
		  nSamplesUsed = Spkr_SubmitWaveBuffer(g_pSpeakerBuffer, g_nBufferIdx);

	  	_ASSERT(nSamplesUsed <= g_nBufferIdx);
	  	if(nSamplesUsed == 0) return;
	  	memmove(g_pSpeakerBuffer, &g_pSpeakerBuffer[nSamplesUsed], g_nBufferIdx-nSamplesUsed);
	  	g_nBufferIdx -= nSamplesUsed;
	  }
  }
}

//=============================================================================

//-----------------------------------------------------------------------------
static ULONG Spkr_SubmitWaveBuffer(short* pSpeakerBuffer, ULONG nNumSamples)
{
	// submit nNumSamples (== 2bytes long each (sizeof short))??
	// from pSpeakerBuffer to pDSSpkrBuf for callback DSPlaySnd

	if(!g_bSpkrRecentlyActive) return nNumSamples;//if not active, just return?

	if(pSpeakerBuffer == NULL)
	{
	// just init sound buffer and cursors??
		return 0;
	}

// conver mono Speakers sounds to stereo (mainly for Mockingboard support)
	UINT len = nNumSamples * 2;	// stereo = 2 * mono
	UINT i;

	for(i = 0; i < len; i += 2)
		g_pStereoBuffer[i] = g_pStereoBuffer[i + 1] = pSpeakerBuffer[i >> 1];

// use code from OpenMSX
//	DSUploadBuffer(pSpeakerBuffer, nNumSamples);
	DSUploadBuffer(g_pStereoBuffer, len);	// submit stereo wave data
	return nNumSamples;	// always return as if we've filled everything!? --bb
}


///////////// Mute - set volume to MINIMUM,	Demute - set volume to NORMAL STATE? -bb
void Spkr_Mute()
{
	SDL_PauseAudio(1);	// dangerous functiouse - will mute Mockingboard, too. Need to be changed
}

void Spkr_Demute()
{
	SDL_PauseAudio(0);
}

//-----------------------------------------------------------------------------


static void Spkr_SetActive(bool bActive)
{		// yes, I know the right way is: 	g_bSpkrRecentlyActive = bActive;, but... ^_^ --bb
	if(bActive)
	{
		// Called by SpkrToggle() or SpkrReset()
		g_bSpkrRecentlyActive = true;

	}
	else
	{
		// Called by SpkrUpdate() after 0.2s of speaker inactivity
		g_bSpkrRecentlyActive = false;
	}
}

bool Spkr_IsActive()
{
	return g_bSpkrRecentlyActive;
}

//-----------------------------------------------------------------------------
// How to deal with volume in SDL Audio
// may be need to go to SDL Mixer?
DWORD SpkrGetVolume()
{
//	return SpeakerVoice.dwUserVolume;
 return 0;
}

void SpkrSetVolume(DWORD dwVolume, DWORD dwVolumeMax)
{
/*	SpeakerVoice.dwUserVolume = dwVolume;

	SpeakerVoice.nVolume = NewVolume(dwVolume, dwVolumeMax);

	if(SpeakerVoice.bActive)
		SpeakerVoice.lpDSBvoice->SetVolume(SpeakerVoice.nVolume);*/
}

//=============================================================================

bool Spkr_DSInit()
{
	//
	// Create single Apple speaker voice
	//
	if(!g_bDSAvailable)
		return false;	// do not have DirectSound? Sorry, SDL Audio! ^_^   --bb
	return true;

}

void Spkr_DSUninit()
{

}

//=============================================================================

DWORD SpkrGetSnapshot(SS_IO_Speaker* pSS)
{
	pSS->g_nSpkrLastCycle = g_nSpkrLastCycle;
	return 0;
}

DWORD SpkrSetSnapshot(SS_IO_Speaker* pSS)
{
	g_nSpkrLastCycle = pSS->g_nSpkrLastCycle;
	return 0;
}

