# Waveshare Audio Driver HAT V2 profile (third-party 4-mic board,
# es7210 2-chip + es8156, X5)
#
# Field description: see audio_hat_v1.profile.
# Board: Waveshare Audio Driver HAT V2. Same codec stack and same X5
# overlay (audio_es7210_es8156_x5_rdk.dts: 2x ES7210 (0x40/0x42) + ES8156
# (0x08)) as the Decenta RDK Audio Kit boards - I2C enumeration, chip IDs
# and POR defaults are register-identical (S100 sub-board identification
# and profile scheme verification record, 2026-10-19), so the boards can
# ONLY be told apart by the user's srpi-config selection. Kept as an
# independent profile so its tuning and channel map evolve separately
# from the Decenta kits (S100 waveshare_hat_v2 lineage).
#
# Channel map (S100 chmap_ws tap-verified 2026-11-16): raw 8ch TDM
# capture dev1: ch0-3 live mics with mic wiring 1->ch1, 2->ch0, 3->ch2,
# 4->ch3 (ch0/ch1 SWAPPED vs the Decenta kit CW 4->1 order); ch4/5 float;
# ch6/7 AEC ref. Playback dev0 (es8156 stereo). Per-mic wiring on X5:
# hw-verify pending (S100-verified order carried over).
#
# Gain baseline: S100 waveshare_hat_v2 board-verified set (42/42 PASS
# 2026-11-16): PGA1-4=14 live mics, PGA5-8=0 noise-floor convention;
# es8156 mapped to X5 kernel-real control names with the X5-calibrated
# DAC Playback Volume=190 + HP Switch=on (S100 DAC=152 is inaudible on
# the X5 HAT speaker). hw-verify pending on an actual X5 Waveshare HAT.
# Requires the CH8 es7210 driver; on a CH4 kernel the ADC5-8/PGA5-8/
# DIRECT_5_6/7_8 lines fail loudly.
#
PROFILE_ID="waveshare_hat_v2"
PROFILE_DISPLAY="Waveshare Audio Driver HAT V2"
PROFILE_VERSION="1.0"
PROFILE_BOARDS="x5_rdk x5_md"
PROFILE_CTRLS="waveshare_hat_v2.ctrl"
PROFILE_ASOUND="waveshare_hat_v2.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
