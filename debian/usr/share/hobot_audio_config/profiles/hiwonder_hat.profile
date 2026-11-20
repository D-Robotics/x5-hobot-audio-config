# Hiwonder Audio Driver HAT profile (X5, hiwonder rasadapter5b)
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
# Board: X5 overlay audio_hiwonder_rasadapter5b_x5_rdk.dts - googlevoicehat
# compatible codec (module snd-soc-googlevoicehat-codec, driver file
# kernel/sound/soc/codecs/googlevoicehat-codec.c) on dw_i2s1.
# The driver declares NO snd_control / DAPM mixer widgets: the only user
# visible switch is the SDMODE GPIO handled inside the driver. There is
# therefore no mixer lines to apply (the .ctrl list exists but is
# intentionally comment-only, so PROFILE_CTRLS stays non-empty for the
# engine's validation contract).
#
# Channel map: mono capture (googlevoicehat is a 1-ch ADC with mono TDM
# out). Channels not re-verified on hardware (hw-verify pending).
#
# gain baseline: driver default, hw-verify pending (no mixer controls
# exist in the driver; nothing to tune from userspace).
#
PROFILE_ID="hiwonder_hat"
PROFILE_DISPLAY="Hiwonder Audio Driver HAT"
PROFILE_VERSION="1.0"
PROFILE_BOARDS="x3_rdk_v2 x3_md x5_rdk x5_md"
PROFILE_CTRLS="hiwonder_hat.ctrl"
PROFILE_ASOUND="hiwonder_hat.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
