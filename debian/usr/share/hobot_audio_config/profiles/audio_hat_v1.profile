# Audio Driver HAT V1 profile (es7210 + es8156; X5 overlay topology,
# also offered on x3 boards where the V1 HAT originates)
#
# Sourced by hobot-audio-profile. Fields:
#   PROFILE_ID       must equal the file name (without .profile)
#   PROFILE_DISPLAY  menu label shown by srpi-config
#   PROFILE_VERSION  bump on every tuning change
#   PROFILE_BOARDS   space separated board markers this profile is offered
#                    on; each token is matched as a PREFIX of srpi-config
#                    get_rdk_type_string ("x5_rdk" matches "x5_rdk_v0" etc.)
#   PROFILE_CTRLS    explicit control list (one amixer line per control)
#   PROFILE_ASOUND   named-PCM contract installed to /etc/asound.conf
#   PROFILE_REGS     optional "chip:reg=val" script, whitelist-checked
#   PROFILE_PA       optional pulseaudio fragment in this dir
#
# Board: RDK X5 + "Audio Driver HAT V1" (legacy srpi-config label, hardware
# derived from the x3_rdk_v1 Audio Driver HAT, X5 overlay
# audio_es7210_es8156_x5_rdk.dts: 2x ES7210 (0x40/0x42) + ES8156 (0x08)).
#
# Channel map: 4-mic board, same hardware lineage as the S100
# rdk_audio_kit_4mic (Decenta RDK Audio Kit 4-Mic, cursor_mk/chmap_4mic):
# mic ch0-3 = CW 4->1; ch4/5 float; ch6/7 AEC ref. Same overlay (and same
# PCM device numbers) as the RDK Audio Kit profiles; capture-side topology
# hw-verified on the 6-mic ring sibling (2026-11-19). Per-mic wiring of a
# physical V1 HAT: hw-verify pending; the .ctrl gain baseline is the S100
# 4-mic board-verified set.
#
# Gain baseline: S100 rdk_audio_kit_4mic board-verified control list
# (42/42 PASS 2026-11-14), mapped onto the X5 kernel driver control names
# (kernel/sound/soc/codecs/es8156.c: "ALC Maximum Minimum Volume" /
# "DAC Playback Volume" instead of the S100 userspace names "ALC Maximum
# Minimum" / "DAC"). hw-verify pending.
#
PROFILE_ID="audio_hat_v1"
PROFILE_DISPLAY="Audio Driver HAT V1"
PROFILE_VERSION="1.1"
# Same overlay (audio_es7210_es8156_x5_rdk.dts) and codec stack as the
# RDK Audio Kit / Waveshare profiles; the X5 legacy menu exposes
# "Audio Driver HAT V1" as well, so this profile must be selectable on
# x5 boards (4-mic gain convention).
PROFILE_BOARDS="x3_rdk_v1 x3_rdk_v2 x3_md x5_rdk x5_md"
PROFILE_CTRLS="audio_hat_v1.ctrl"
PROFILE_ASOUND="audio_hat_v1.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
