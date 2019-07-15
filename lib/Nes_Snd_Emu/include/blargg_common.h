// Sets up common environment for Shay Green's libraries.

// Copyright (C) 2004-2005 Shay Green.

#pragma once

// Common standard headers
#include <cstdint>
#include <climits>
#include <cstddef>
#include <cassert>

// blargg_err_t (NULL on success, otherwise error string)
typedef const char* blargg_err_t;
const blargg_err_t blargg_success = nullptr;

// If expr returns non-NULL error string, return it from current function, otherwise continue.
#define BLARGG_RETURN_ERR( expr ) do {                          \
		blargg_err_t blargg_return_err_ = (expr);               \
		if ( blargg_return_err_ ) return blargg_return_err_;    \
	} while ( 0 )

// If ptr is NULL, return out of memory error string.
#define BLARGG_CHECK_ALLOC( ptr )   do { if ( !(ptr) ) return "Out of memory"; } while ( 0 )