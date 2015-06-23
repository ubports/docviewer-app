/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// This short example shows how to use LibreOfficeKit to render (parts of) a
// document. As-is, it renders a full view of a document into a 250x250 pixel
// png, ignoring aspect ratio. Its using hardcoded Ubuntu/Debian paths to find
// the LibreOffice installation.
//
// Please install prerequisites if you havent done so:
//
//     sudo apt install libreofficekit-dev libreoffice libpng12-dev
//
// Note that you need LibreOffice 5.0 -- a prerelease is currently available 
// at the prereleases ppa:
//
//     https://launchpad.net/~libreoffice/+archive/ubuntu/libreoffice-prereleases
//
// That ppa has versions for Ubuntu 15.10, 15.04, 14.04 LTS and 12.04 LTS.
// Although this should work on all releases, it has only been tested on Ubuntu
// 15.04 (vivid) so far.
//
// This example comes without a build system as presumably you would want to
// use a different one anyway.
// To compile/link this, save this file as libreoffice2png.cxx and run:
/*
        g++ --std=c++11 -I/usr/include/LibreOfficeKit/ `pkg-config --cflags libpng` libreoffice2png.cxx `pkg-config --libs libpng` -ldl -o ./libreoffice2png
        # or run this sh pipe : grep g++ ./libreoffice2png.cxx | sh
*/
// You should be able to easily teach your build system of choice how to build
// based on the above.


#include <iostream>
#include <memory>
#include "png.h"
#include "assert.h"
#define LOK_USE_UNSTABLE_API
#include "LibreOfficeKitInit.h"
#include "LibreOfficeKit.hxx"

constexpr png_uint_32 nCanvasHeight = 256;
constexpr png_uint_32 nCanvasWidth = 256;
constexpr png_uint_32 nCanvasDepth = 8;
constexpr png_uint_32 nBytesPerPixel = 4;

// a simple RGBA buffer to render a document to
png_byte image[nCanvasHeight][nCanvasWidth*nBytesPerPixel];
void write_png(const char*);

int main(int argc, char** argv)
{
	long nDocWidth(0);
	long nDocHeight(0);
	if(argc != 3)
	{
		std::cerr << "libreoffice2png renders a document to a png file." << std::endl;
		std::cerr << "usage: ./libreoffice2png <input-document> <output-png>" << std::endl;
		return -1;
	}
	std::unique_ptr<lok::Office> pOffice(lok::lok_cpp_init("/usr/lib/libreoffice/program/"));
	std::unique_ptr<lok::Document> pDocument(pOffice->documentLoad(argv[1]));
	pDocument->initializeForRendering();
	std::cout << "Document type: " << pDocument->getDocumentType() << std::endl;
	pDocument->getDocumentSize(&nDocWidth, &nDocHeight);
	std::cout << "Document size: " << nDocWidth << "x" << nDocHeight << " twips" << std::endl;
	pDocument->paintTile(&image[0][0], nCanvasHeight, nCanvasWidth, 0, 0, nDocWidth, nDocHeight);
	write_png(argv[2]);
	pDocument.reset(nullptr);
	pOffice.reset(nullptr);
	return 0;
}

// only png writer code below this
#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif

void write_png(const char* file_name)
{

	FILE* fp = fopen(file_name, "wb");
	if (!fp)
		return;

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

	if (!png_ptr)
	{
		fclose(fp);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, nullptr);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return;
	}

	png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr, nCanvasWidth, nCanvasHeight, nCanvasDepth, PNG_COLOR_TYPE_RGB_ALPHA,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	assert(nCanvasHeight <= PNG_UINT_32_MAX/png_sizeof(png_bytep));
	png_bytep row_pointers[nCanvasHeight];

	for (png_uint_32 row = 0; row < nCanvasHeight; ++row)
		row_pointers[row] = &(image[row][0]);

	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
}
/* vim: set noet sw=4 ts=4: */
