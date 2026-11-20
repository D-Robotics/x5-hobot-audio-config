# Audio Driver HAT V2 4-Mic profile (es7210 2-chip + es8156, X5)
#
# Field description: see audio_hat_v1.profile.
# Board: X5 overlay audio_es7210_es8156_x5_rdk.dts: 2x ES7210 (0x40/0x42)
# + ES8156 (0x08), same codec stack as the 6-Mic ring variant; differs only
# in populated microphones (4) and therefore gain ownership per channel.
#
# Channel map (hw-verified on the 6mic sibling 2026-11-19, capture side):
# capture dev1 (es7210 8ch TDM): ch1-4 live mics, ch5/6 float, ch7/8 AEC
# ref; playback dev0 (es8156 stereo).
#
# Gain baseline: PGA1-4=14 (S100 rdk_audio_kit_4mic board-verified live
# values), PGA5-8=0 (noise-floor convention for unpopulated channels);
# es8156 from S100 waveshare_hat_v2 board-verified set. hw-verify pending
# on an actual X5 4-mic HAT.
#
PROFILE_ID="audio_hat_v2_4mic"
PROFILE_DISPLAY="Audio Driver HAT V2 (4-Mic)"
PROFILE_VERSION="1.1"
PROFILE_BOARDS="x3_rdk_v2 x3_md x5_rdk x5_md"
PROFILE_CTRLS="audio_hat_v2_4mic.ctrl"
PROFILE_ASOUND="audio_hat_v2_4mic.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
