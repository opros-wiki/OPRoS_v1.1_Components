//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
* @file    ersp-objrec/ersp-objrec-poly-check.h
*
* @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
*/

#ifndef __ERSP_OBJREC_POLY_CHECK_H__
#define __ERSP_OBJREC_POLY_CHECK_H__

#include "ersp-objrec-common.h"

ERSP_OBJREC_BEGIN_DECLS

typedef struct
{
    int rising;
    int x1, x2, y1, y2;
} ERSP_PolygonEdge;

typedef struct
{
    int num_edges;
    ERSP_PolygonEdge *edges;
} ERSP_KeypointPolygon;

ERSP_OBJREC_EXPORT ERSP_KeypointPolygon *
ERSP_objrec_new_keypoint_polygon(unsigned n_points, double *xy_pairs);

ERSP_OBJREC_EXPORT int
ERSP_objrec_check_keypoint_polygon(ERSP_KeypointPolygon *poly, double xpos,
                                   double ypos);

ERSP_OBJREC_EXPORT void
ERSP_objrec_delete_keypoint_polygon(ERSP_KeypointPolygon *poly);

ERSP_OBJREC_END_DECLS

#endif
