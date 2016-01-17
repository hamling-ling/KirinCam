#pragma once

#ifndef PI
#define PI  3.14159265358979323846f
#endif
#define EPSILON		(1e-5f)
#define EPSIOLONLOW	(1e-3f)
#define FEQ(x,y)	(fabs((x)-(y))<EPSILON)
#define FEQLOW(x,y)	(fabs((x)-(y))<EPSIOLONLOW)
#define ISPOSITIVE(x)	((x) > 0.0f)

