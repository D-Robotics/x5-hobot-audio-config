# WM8960 Audio Driver HAT profile (X5)
#
# Field description: see hiwonder_hat.profile
# Board: X5 overlay audio_wm8960_x5_rdk.dts - wm8960 (wlf,wm8960) on
# dw_i2s1. Control names verified against
# kernel/sound/soc/codecs/wm8960.c wm8960_snd_controls[].
#
# Initial values follow the in-tree alsactl baseline shipped for x3
# (debian/etc/hobot_audio_config/x3/wm8960_config/wm8960_asound.state,
# control values copied verbatim). Only controls with a meaningful,
# source-verifiable initial value are listed; driver defaults apply to the
# rest. NOTE: on X5 the legacy path does NOT install this state file
# (wm8960_asound.state lives under debian/etc/hobot_audio_config/x3/ and
# mk_debs.sh copies only the x5/ subtree for RDK_SOC_NAME=x5).
#
# gain baseline: driver default, hw-verify pending (values below are the
# x3 baseline; not verified on X5 hardware).
#
PROFILE_ID="wm8960_hat"
PROFILE_DISPLAY="WM8960 Audio Driver HAT"
PROFILE_VERSION="1.0"
PROFILE_BOARDS="x3_rdk_v2 x3_md x5_rdk x5_md"
PROFILE_CTRLS="wm8960_hat.ctrl"
PROFILE_ASOUND="wm8960_hat.asound.conf"
PROFILE_REGS=""
PROFILE_PA=""
