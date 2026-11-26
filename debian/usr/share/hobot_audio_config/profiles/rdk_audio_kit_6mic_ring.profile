# RDK Audio Kit 6-Mic profile (Decenta 6-mic ring board,
# es7210 2-chip + es8156, X5)
#
# Field description: see audio_hat_v1.profile.
# Board: Decenta RDK Audio Kit, 6 ring mics (S100 rdk_audio_kit_6mic_ring
# lineage; hw-verified on X5 MD + this board 2026-11-19). X5 overlay
# audio_es7210_es8156_x5_rdk.dts: 2x ES7210 (0x40/0x42) + ES8156 (0x08).
# Profile id aligned with S100 (rdk_audio_kit_6mic_ring); the legacy X5 id
# audio_hat_v2_6mic is migrated by hobot-audio-config postinst (selection
# memory + module auto-load entry) and accepted as a nonint alias by
# srpi-config.
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
PROFILE_ID="rdk_audio_kit_6mic_ring"
PROFILE_DISPLAY="RDK Audio Kit 6-Mic"
PROFILE_VERSION="1.2"
PROFILE_BOARDS="x3_rdk_v2 x3_md x5_rdk x5_md"
PROFILE_CTRLS="rdk_audio_kit_6mic_ring.ctrl"
PROFILE_ASOUND="rdk_audio_kit_6mic_ring.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
