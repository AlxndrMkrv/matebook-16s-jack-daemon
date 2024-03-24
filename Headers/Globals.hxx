#pragma once

static constexpr const char * ProgramName = "matebook-16s-sound-reconf";
static constexpr const char * EventSource = "sof-hda-dsp Headphone";
static constexpr const char * SoC = "SN6140";
static constexpr const char * DevInputDir = "/dev/input";
static constexpr const char * SysInputDir = "/sys/class/input";
static constexpr const char * DevSoundDir = "/dev/snd";
static constexpr const char * SysSoundDir = "/sys/class/sound";
static constexpr unsigned short EventType = 5;
static constexpr unsigned short EventCode = 2;
