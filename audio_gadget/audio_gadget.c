/*
 * Copyright (c) 2024，D-Robotics.
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

#define MAX_LINE_LENGTH 256

typedef struct
{
    char module[50];
    char device[50];
    int mmap;
    int tsched;
    int fragments;
    int fragment_size;
    unsigned int rate;
    unsigned int channels;
    int rewind_safeguard;
} AudioConfig;


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

int find_card_for_control(const char *control_name, long volume, char *found_card) {
    char card_name[32];
    int card_index = -1;

    while (snd_card_next(&card_index) >= 0 && card_index >= 0) {
        snprintf(card_name, sizeof(card_name), "hw:%d", card_index);

        if (set_control_value(control_name, volume, card_name) == 0) {
            strcpy(found_card, card_name);
            return 0;  // find it return 0
        }
    }

    return -1;  // not find
}

void parseLine(char *line, AudioConfig *config)
{
    sscanf(line, "load-module %s device=%s mmap=%d tsched=%d fragments=%d fragment_size=%d rate=%d channels=%d rewind_safeguard=%d",
           config->module, config->device, &config->mmap, &config->tsched, &config->fragments, &config->fragment_size,
           &config->rate, &config->channels, &config->rewind_safeguard);
}

void openPlaybackDevice(AudioConfig *config, snd_pcm_t **playback_handle)
{
    int err;
    snd_pcm_hw_params_t *params;

    printf("Opening playback device: %s\n", config->device);

    err = snd_pcm_open(playback_handle, config->device, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0)
    {
        fprintf(stderr, "Error opening playback device: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    // Allocate hardware parameters structure
    snd_pcm_hw_params_alloca(&params);

    // Fill it in with default values
    snd_pcm_hw_params_any(*playback_handle, params);

    // Set the desired hardware parameters
    snd_pcm_hw_params_set_access(*playback_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(*playback_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(*playback_handle, params, config->channels);
    snd_pcm_hw_params_set_rate_near(*playback_handle, params, &config->rate, 0);

    // Write the parameters to the driver
    err = snd_pcm_hw_params(*playback_handle, params);
    if (err < 0)
    {
        fprintf(stderr, "Error setting hardware parameters: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
}

void playBlankAudio(snd_pcm_t *playback_handle)
{
    // Sample data: play a silent sound
    const size_t frames = 48000; // Adjust based on your requirements
    short buffer[frames * 2];    // 2 channels

    printf("Playing blank audio...\n");

    // // Write the silent sound to the PCM device
    if (snd_pcm_writei(playback_handle, buffer, frames) < 0)
    {
        fprintf(stderr, "Error playing audio\n");
    }
}

void closePlaybackDevice(snd_pcm_t *playback_handle)
{
    printf("Closing playback device...\n");
    snd_pcm_close(playback_handle);
}

void openRecordingDevice(AudioConfig *config, snd_pcm_t **capture_handle)
{
    int err;
    snd_pcm_hw_params_t *params;

    printf("Opening recording device: %s\n", config->device);

    err = snd_pcm_open(capture_handle, config->device, SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0)
    {
        fprintf(stderr, "Error opening recording device: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    // Allocate hardware parameters structure
    snd_pcm_hw_params_alloca(&params);

    // Fill it in with default values
    snd_pcm_hw_params_any(*capture_handle, params);

    // Set the desired hardware parameters
    snd_pcm_hw_params_set_access(*capture_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(*capture_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(*capture_handle, params, config->channels);
    snd_pcm_hw_params_set_rate_near(*capture_handle, params, &config->rate, 0);

    // Write the parameters to the driver
    err = snd_pcm_hw_params(*capture_handle, params);
    if (err < 0)
    {
        fprintf(stderr, "Error setting hardware parameters: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
}

void recordAudio(snd_pcm_t *capture_handle)
{
    // Sample data: record audio but don't save it
    const size_t frames = 1; // Adjust based on your requirements
    short buffer[frames];    // 2 channels

    printf("Recording audio (not saving)...\n");

    // Read audio from the PCM device
    if (snd_pcm_readi(capture_handle, buffer, frames) < 0)
    {
        fprintf(stderr, "Error recording audio\n");
    }
}

void closeRecordingDevice(snd_pcm_t *capture_handle)
{
    printf("Closing recording device...\n");
    snd_pcm_close(capture_handle);
}

int main()
{
    const char *cur_audio_hat_path = "/etc/hobot_audio_config/cur_audio_hat";
    /*for es8326*/
    const char *control_name = "ADC PGA Gain";
    char detected_card[32];
    int value_adc_pga_gain = 8;

    FILE *cur_audio_hat_file;

    // Check if the cur_audio_hat_file exists
    if ((cur_audio_hat_file = fopen(cur_audio_hat_path, "r")) != NULL)
    {
        // File exists
        fseek(cur_audio_hat_file, 0, SEEK_END);
        long unsigned int file_size = ftell(cur_audio_hat_file);

        if (file_size > 0)
        {
            // File is not empty
            rewind(cur_audio_hat_file);

            // Read cur_audio_hat_file content
            char buffer[file_size + 1];
            if (fread(buffer, 1, file_size, cur_audio_hat_file) == file_size)
            {
                buffer[file_size] = '\0'; // Append null character at the end

                // Check if the cur_audio_hat_file content is "UNSET"
                if (strcmp(buffer, "UNSET") == 0)
                {
                    printf("File exists and content is UNSET\n");
                    fclose(cur_audio_hat_file);
                    return 0;
                }
                else
                {
                    printf("File exists, not empty, and content is not UNSET\n");
                }
            }
            else
            {
                fprintf(stderr, "Unable to read cur_audio_hat_file content\n");
            }
        }
        else
        {
            // File exists but is empty
            printf("File exists but is empty\n");
            fclose(cur_audio_hat_file);
            return 0;
        }

        fclose(cur_audio_hat_file);
    }
    else
    {
        // File does not exist
        printf("File does not exist , No HAT setting start!\n");
        if (find_card_for_control(control_name, value_adc_pga_gain, detected_card) == 0) {
            printf("Control found on %s\n", detected_card);
        } else {
            printf("Control not found on any card\n");
        }

        return 0;
    }

    FILE *file = fopen("/etc/pulse/default.pa", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    AudioConfig playbackConfig, recordingConfig;
    snd_pcm_t *playback_handle = NULL;
    snd_pcm_t *capture_handle = NULL;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line[0] == '#')
        {
            // Skip lines starting with #
            continue;
        }
        if (strstr(line, "module-alsa-sink") != NULL)
        {
            parseLine(line, &playbackConfig);
            openPlaybackDevice(&playbackConfig, &playback_handle);
            playBlankAudio(playback_handle);
            closePlaybackDevice(playback_handle);
        }
        if (strstr(line, "module-alsa-source") != NULL)
        {
            parseLine(line, &recordingConfig);
            openRecordingDevice(&recordingConfig, &capture_handle);
            recordAudio(capture_handle);
            closeRecordingDevice(capture_handle);
        }
    }

    fclose(file);

    return 0;
}
