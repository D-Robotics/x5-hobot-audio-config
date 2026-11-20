# Audio Driver HAT V2 6-Mic ring profile (es7210 2-chip + es8156, X5)
#
# Field description: see audio_hat_v1.profile.
# Board: X5 overlay audio_es7210_es8156_x5_rdk.dts: 2x ES7210 (0x40/0x42)
# + ES8156 (0x08). 6-Mic ring hardware lineage of the S100
# rdk_audio_kit_6mic_ring.
#
# Channel map (hw-verified on X5 MD + 6mic ring 2026-11-19): capture dev1
# (es7210 8ch TDM): ch1-6 live mics (ring), ch7/8 AEC ref slots (digital
# silence in raw capture); playback dev0 (es8156 stereo).
#
# Gain baseline: board-verified live on X5 (2026-11-19): PGA1-6=14 gives
# six LIVE channels (per-channel RMS 233-264 @16k/8ch), PGA7/8=0
# (unpopulated); mirrors S100 rdk_audio_kit_6mic_ring. es8156 values from
# S100 waveshare_hat_v2 board set (kernel-real names).
#
PROFILE_ID="audio_hat_v2_6mic"
PROFILE_DISPLAY="Audio Driver HAT V2 (6-Mic Ring)"
PROFILE_VERSION="1.1"
PROFILE_BOARDS="x3_rdk_v2 x3_md x5_rdk x5_md"
PROFILE_CTRLS="audio_hat_v2_6mic.ctrl"
PROFILE_ASOUND="audio_hat_v2_6mic.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
