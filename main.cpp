#include <iostream>
#include <alsa/asoundlib.h>

int main(int argc, char **argv) {
    int i;
    int err;
    short buf[128];
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    
    // device name argv[1]: default

    if ((err = snd_pcm_open (&playback_handle, argv[1], SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
	    std::cerr << "cannot open audio device " << argv[1] << " (" << snd_strerror(err) << ")" << std::endl;
	    exit(1);
    }
    
    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
	    std::cerr << "cannot allocate hardware parameter structure " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {
	    std::cerr << "cannot initialize hardware parameter structure " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
	    std::cerr << "cannot set access type " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
	    std::cerr << "cannot set sample format: " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    unsigned int val = 44100;
    int dir = 0;
    
    if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &val, &dir)) < 0) {
	    std::cerr << "cannot set sample rate: " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2)) < 0) {
	    std::cerr << "cannot set channel count: " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
	    std::cerr << "cannot set parameters: " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    snd_pcm_hw_params_free(hw_params);
    
    if ((err = snd_pcm_prepare(playback_handle)) < 0) {
	    std::cerr << "cannot prepare audio interface for use: " << snd_strerror(err) << std::endl;
	    exit(1);
    }
    
    for (i = 0; i < 8000; ++i) {
	    if ((err = snd_pcm_writei(playback_handle, buf, 128)) != 128) {
	        std::cerr << "write to audio interface failed: " << snd_strerror(err) << std::endl;
	        exit(1);
	    }
    }
    
    snd_pcm_close(playback_handle);
    return 0;
    
}
