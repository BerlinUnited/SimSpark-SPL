#ifndef EXPORT_LIB
#define EXPORT_LIB
/**
 * \file soccercontrolframe/export.h
 * \brief Exports the SoccerControlFrame class as a plugin from a shared library.
 */

#include "soccercontrolframe.h"

PLUGINS_EXPORT_BEGIN()
PLUGIN_EXPORT(SoccerControlFrame)
PLUGINS_EXPORT_END()

#endif