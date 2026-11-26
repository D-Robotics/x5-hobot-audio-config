# RDK Audio Kit 4-Mic profile (Decenta 4-mic parallel board,
# es7210 2-chip + es8156, X5)
#
# Field description: see audio_hat_v1.profile.
# Board: Decenta RDK Audio Kit, 4 parallel mics (S100 rdk_audio_kit_4mic
# lineage). X5 overlay audio_es7210_es8156_x5_rdk.dts: 2x ES7210 (0x40/0x42)
# + ES8156 (0x08), same codec stack as the 6-Mic ring variant; differs only
# in populated microphones (4) and therefore gain ownership per channel.
# Profile id aligned with S100 (rdk_audio_kit_4mic); the legacy X5 id
# audio_hat_v2_4mic is migrated by hobot-audio-config postinst (selection
# memory + module auto-load entry) and accepted as a nonint alias by
# srpi-config. The Waveshare HAT V2 is a DIFFERENT board with its own
# profile (waveshare_hat_v2).
#
# Channel map (S100 chmap_4mic board-verified 2026-11-14, capture side;
# X5 topology hw-verified on the 6mic sibling 2026-11-19):
# capture dev1 (es7210 8ch TDM): ch1-4 live mics (CW 4->1), ch5/6 float,
# ch7/8 AEC ref; playback dev0 (es8156 stereo).
#
# Gain baseline: PGA1-4=14 (S100 rdk_audio_kit_4mic board-verified live
# values), PGA5-8=0 (noise-floor convention for unpopulated channels);
# es8156 from S100 board-verified set, X5-calibrated DAC=190. hw-verify
# pending on an actual X5 4-mic kit.
#
PROFILE_ID="rdk_audio_kit_4mic"
PROFILE_DISPLAY="RDK Audio Kit 4-Mic"
PROFILE_VERSION="1.2"
PROFILE_BOARDS="x3_rdk_v2 x3_md x5_rdk x5_md"
PROFILE_CTRLS="rdk_audio_kit_4mic.ctrl"
PROFILE_ASOUND="rdk_audio_kit_4mic.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
