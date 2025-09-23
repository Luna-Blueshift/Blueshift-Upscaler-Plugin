// Copyright (c) Blueshift Interactive Ltd (2025)

#pragma once

/// DLSS defines
#ifndef WITH_DLSS
#define WITH_DLSS 0
#endif


/// FSR defines
#ifndef WITH_FSR4
#define WITH_FSR4 0
#endif

#ifndef WITH_FSR3
#define WITH_FSR3 0
#endif

#define WITH_FSR WITH_FSR3 || WITH_FSR4


/// XeSS defines
#ifndef WITH_XESS
#define WITH_XESS 0
#endif