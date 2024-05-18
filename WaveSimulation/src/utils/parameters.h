#pragma once

#define N_SPATIAL 128 // number of spatial disc. nodes
#define N_THETA 16	  // number of wave angle disc. nodes
#define N_K 1		  // number of wave length disc. nodes

static_assert(N_SPATIAL % 16 == 0); // Assert that spatial dim is divisible by 16
static_assert(N_THETA % 4 == 0); // Assert that number of directions is divisible by 4