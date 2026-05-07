/*
 * Copyright (c) 2024, D-Robotics.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>

int set_control_value(const char *control_name, long value, const char *card_name) {
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;

    // open mixer
    if (snd_mixer_open(&handle, 0) < 0) {
        fprintf(stderr, "Failed to open mixer\n");
        return -1;
    }

    if (snd_mixer_attach(handle, card_name) < 0) {
        fprintf(stderr, "Failed to attach mixer to card %s\n", card_name);
        snd_mixer_close(handle);
        return -1;
    }

    if (snd_mixer_selem_register(handle, NULL, NULL) < 0) {
        fprintf(stderr, "Failed to register mixer\n");
        snd_mixer_close(handle);
        return -1;
    }

    if (snd_mixer_load(handle) < 0) {
        fprintf(stderr, "Failed to load mixer\n");
        snd_mixer_close(handle);
        return -1;
    }

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_name(sid, control_name);

    elem = snd_mixer_find_selem(handle, sid);
    if (!elem) {
        fprintf(stderr, "Unable to find control '%s' on card %s\n", control_name, card_name);
        snd_mixer_close(handle);
        return -1;
    }

    snd_mixer_selem_set_playback_volume_all(elem, value);

    snd_mixer_close(handle);

    return 0;
}

void playBlankAudio(snd_pcm_t *playback_handle)
{
    // Sample data: play a silent sound
    const size_t frames = 48000 * 0.1; // Adjust based on your requirements
    short buffer[frames * 2];    // 2 channels

    memset(buffer, 0, sizeof(buffer));

    printf("Playing blank audio...\n");

    // // Write the silent sound to the PCM device
    snd_pcm_writei(playback_handle, buffer, frames);
}

void closePlaybackDevice(snd_pcm_t *playback_handle)
{
    printf("Closing playback device...\n");
    snd_pcm_drop(playback_handle);
    snd_pcm_hw_free(playback_handle);
    snd_pcm_close(playback_handle);
}

void recordAudio(snd_pcm_t *capture_handle)
{
    // Sample data: record audio but don't save it
    const size_t frames = 48000 * 0.1; // Adjust based on your requirements
    short buffer[frames*2];    // 2 channels

    printf("Recording audio (not saving)...\n");

    // Read audio from the PCM device
    snd_pcm_readi(capture_handle, buffer, frames);
}

void closeRecordingDevice(snd_pcm_t *capture_handle)
{
    printf("Closing recording device...\n");
    snd_pcm_drop(capture_handle);
    snd_pcm_hw_free(capture_handle);
    snd_pcm_close(capture_handle);
}

int main()
{
    int card = -1;
    const char *control_name = "ADC PGA Gain";
    int value_adc_pga_gain = 8;

    printf("Scanning all sound cards...\n");

    while (snd_card_next(&card) >= 0 && card >= 0) {
        char device_name[32];
        snprintf(device_name, sizeof(device_name), "hw:%d", card);
        printf("Card %d -> %s\n", card, device_name);

        if (set_control_value(control_name, value_adc_pga_gain, device_name) == 0) {
            printf("Control found on %s\n", device_name);
        }

        snd_ctl_t *ctl;
        char ctl_name[32];
        snprintf(ctl_name, sizeof(ctl_name), "hw:%d", card);
        if (snd_ctl_open(&ctl, ctl_name, 0) < 0) continue;

        int device = -1;
        while (snd_ctl_pcm_next_device(ctl, &device) >= 0 && device >= 0) {
            char pcm_name[32];
            snprintf(pcm_name, sizeof(pcm_name), "hw:%d,%d", card, device);

            int has_playback = 0;
            int has_capture = 0;
            snd_pcm_info_t *pcminfo;
            snd_pcm_info_alloca(&pcminfo);

            snd_pcm_info_set_device(pcminfo, device);
            snd_pcm_info_set_subdevice(pcminfo, 0);

            snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_PLAYBACK);
            if (snd_ctl_pcm_info(ctl, pcminfo) >= 0)
                has_playback = 1;

            snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_CAPTURE);
            if (snd_ctl_pcm_info(ctl, pcminfo) >= 0)
                has_capture = 1;

            printf("Found PCM device: %s, has_playback: %d, has_capture: %d\n", pcm_name, has_playback, has_capture);

            if (has_playback) {
                snd_pcm_t *play_handle;
                if (snd_pcm_open(&play_handle, pcm_name, SND_PCM_STREAM_PLAYBACK, 0) >= 0) {
                    int err;
                    printf("Playing test tone on %s\n", pcm_name);
                    err = snd_pcm_set_params(play_handle,
                                    SND_PCM_FORMAT_S16_LE,
                                    SND_PCM_ACCESS_RW_INTERLEAVED,
                                    2,
                                    48000,
                                    1,
                                    50000); // 0.5s latency
                    if (err < 0) {
                        fprintf(stderr, "Set playback params failed on %s: %s\n",
                                pcm_name, snd_strerror(err));
                        closePlaybackDevice(play_handle);
                        continue;
                    }
                    playBlankAudio(play_handle);
                    closePlaybackDevice(play_handle);
                }
            }

            if (has_capture) {
                snd_pcm_t *capture_handle;
                if (snd_pcm_open(&capture_handle, pcm_name, SND_PCM_STREAM_CAPTURE, 0) >= 0) {
                    int err;
                    printf("Recording 1s from %s\n", pcm_name);
                    err = snd_pcm_set_params(capture_handle,
                                    SND_PCM_FORMAT_S16_LE,
                                    SND_PCM_ACCESS_RW_INTERLEAVED,
                                    2,
                                    48000,
                                    1,
                                    50000);
                    if (err < 0) {
                        fprintf(stderr, "Set capture params failed on %s: %s\n",
                                pcm_name, snd_strerror(err));
                        closeRecordingDevice(capture_handle);
                        continue;
                    }
                    recordAudio(capture_handle);
                    closeRecordingDevice(capture_handle);
                }
            }
        }
        snd_ctl_close(ctl);
    }

    return 0;
}
