#include "audio.h"

void CALLBACK WaveOutProc(HWAVEOUT wave_out_handle, UINT message,
                          DWORD_PTR instance, DWORD_PTR param1,
                          DWORD_PTR param2) {
    WavData *wavdata = (WavData *)instance;

	switch(message) {
        case WOM_CLOSE: {
            break;
        }
        case WOM_OPEN:  {
            break;
        }
		case WOM_DONE:{ 
            if(!wavdata->play || !AUDIO_START) break;
			for(int i = 0; i < CHUNK_SIZE; ++i) {
                wavdata->handleStream(&wavdata->chunks[wavdata->chunk_swap][i], wavdata);
			}
			if(waveOutWrite(wavdata->wave_out, &wavdata->header[wavdata->chunk_swap], sizeof(wavdata->header[wavdata->chunk_swap]))
                            != MMSYSERR_NOERROR) {
				PERROR("waveOutWrite failed\n");
			}
			wavdata->chunk_swap = !wavdata->chunk_swap;
            break;
		}
	}
}

int wav_init(WavData *wavdata, void (*initStream) (struct _WavData *wavdata), 
            void (*handleStream) (int16_t *stream, struct _WavData *wavdata),
            WAVEFORMATEX *format, void *data) 
{
    wavdata->chunk_swap = 0;
    wavdata->data = data;
    wavdata->initStream = initStream;
    wavdata->handleStream = handleStream;
    wavdata->play = false;

    WAVEHDR *header = wavdata->header;

    format->nBlockAlign = format->nChannels * format->wBitsPerSample / 8;
    format->nAvgBytesPerSec = format->nSamplesPerSec * format->nBlockAlign;

    // WAVE_MAPPER automatically selects the best device
    // would probably be a good idea to have the option to actuall selec the
    // device in the future
    if(waveOutOpen(&wavdata->wave_out, WAVE_MAPPER, format, (DWORD_PTR)WaveOutProc,
                (DWORD_PTR)wavdata, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
        PERROR("waveOutOpen failed\n");
        return -1;
    }

    // allows you to set the volume in the mixer
	if(waveOutSetVolume(wavdata->wave_out, 0xFFFFFFFF) != MMSYSERR_NOERROR) {
		PERROR("waveOutGetVolume failed\n");
		return -1;
	}

    // you don't need to process the audio twice, you just need to initialize
    // your data and send it to waveOutPrepareHeader and waveOutWrite
    memset(header, 0, sizeof(WAVEHDR) * CHUNK_COUNT);
    memset(&wavdata->chunks, 0, sizeof(int16_t) * CHUNK_SIZE * CHUNK_COUNT);

    for (int i = 0; i < CHUNK_COUNT; ++i) {
    	header[i].lpData = (char *)wavdata->chunks[i];
    	header[i].dwBufferLength = CHUNK_SIZE * CHUNK_COUNT;
        if(waveOutPrepareHeader(wavdata->wave_out, &header[i], sizeof(header[i])) != MMSYSERR_NOERROR) {
            PERROR("waveOutPrepareHeader[%d] failed\n", i);
            return -1;
        }
        if(waveOutWrite(wavdata->wave_out, &header[i], sizeof(header[i])) != MMSYSERR_NOERROR) {
            PERROR("waveOutWrite[%d] failed\n", i);
            return -1;
        }
        
    }
    wavdata->format = format;
    wavdata->initStream(wavdata);

	return 0;
}
