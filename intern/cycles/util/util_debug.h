/*
 * Copyright 2011-2013 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __UTIL_DEBUG_H__
#define __UTIL_DEBUG_H__

#include <cassert>
#include <iostream>

#include "util_static_assert.h"
#include "util_vector.h"

#ifdef WITH_CYCLES_DEBUG_FILTER
#include "OpenImageIO/imageio.h"
OIIO_NAMESPACE_USING
#endif

CCL_NAMESPACE_BEGIN

/* Global storage for all sort of flags used to fine-tune behavior of particular
 * areas for the development purposes, without officially exposing settings to
 * the interface.
 */
class DebugFlags {
public:
	/* Descriptor of CPU feature-set to be used. */
	struct CPU {
		CPU();

		/* Reset flags to their defaults. */
		void reset();

		/* Flags describing which instructions sets are allowed for use. */
		bool avx2;
		bool avx;
		bool sse41;
		bool sse3;
		bool sse2;

		/* Whether QBVH usage is allowed or not. */
		bool qbvh;
	};

	/* Descriptor of CUDA feature-set to be used. */
	struct CUDA {
		CUDA();

		/* Reset flags to their defaults. */
		void reset();

		/* Whether adaptive feature based runtime compile is enabled or not.
		 * Requires the CUDA Toolkit and only works on Linux atm. */
		bool adaptive_compile;
	};

	/* Descriptor of OpenCL feature-set to be used. */
	struct OpenCL {
		OpenCL();

		/* Reset flags to their defaults. */
		void reset();

		/* Available device types.
		 * Only gives a hint which devices to let user to choose from, does not
		 * try to use any sort of optimal device or so.
		 */
		enum DeviceType {
			/* None of OpenCL devices will be used. */
			DEVICE_NONE,
			/* All OpenCL devices will be used. */
			DEVICE_ALL,
			/* Default system OpenCL device will be used.  */
			DEVICE_DEFAULT,
			/* Host processor will be used. */
			DEVICE_CPU,
			/* GPU devices will be used. */
			DEVICE_GPU,
			/* Dedicated OpenCL accelerator device will be used. */
			DEVICE_ACCELERATOR,
		};

		/* Available kernel types. */
		enum KernelType {
			/* Do automated guess which kernel to use, based on the officially
			 * supported GPUs and such.
			 */
			KERNEL_DEFAULT,
			/* Force mega kernel to be used. */
			KERNEL_MEGA,
			/* Force split kernel to be used. */
			KERNEL_SPLIT,
		};

		/* Requested device type. */
		DeviceType device_type;

		/* Requested kernel type. */
		KernelType kernel_type;

		/* Use debug version of the kernel. */
		bool debug;
	};

	/* Get instance of debug flags registry. */
	static DebugFlags& get()
	{
		static DebugFlags instance;
		return instance;
	}

	/* Reset flags to their defaults. */
	void reset();

	/* Requested CPU flags. */
	CPU cpu;

	/* Requested CUDA flags. */
	CUDA cuda;

	/* Requested OpenCL flags. */
	OpenCL opencl;

private:
	DebugFlags();

#if (__cplusplus > 199711L)
public:
	explicit DebugFlags(DebugFlags const& /*other*/)     = delete;
	void operator=(DebugFlags const& /*other*/) = delete;
#else
private:
	explicit DebugFlags(DebugFlags const& /*other*/);
	void operator=(DebugFlags const& /*other*/);
#endif
};

typedef DebugFlags& DebugFlagsRef;
typedef const DebugFlags& DebugFlagsConstRef;

inline DebugFlags& DebugFlags() {
  return DebugFlags::get();
}

std::ostream& operator <<(std::ostream &os,
                          DebugFlagsConstRef debug_flags);

class DebugPasses {
public:

#ifdef WITH_CYCLES_DEBUG_FILTER
	DebugPasses(int w, int h, int channels, int pixelstride, int linestride)
	: w(w),
	  h(h),
	  pixelstride(pixelstride),
	  linestride(linestride)
	{
		pixels.resize(w*h*channels);
	}

	void add_pass(std::string name, float *data);
	bool write(std::string name);

	int w, h, pixelstride, linestride;
	vector<float> pixels;
	vector<std::string> passnames;
#else
	DebugPasses(int, int, int, int, int) {}
	void add_pass(std::string, float*) {}
	bool write(std::string) { return true; }
#endif
};

CCL_NAMESPACE_END

#endif /* __UTIL_DEBUG_H__ */
