//
// Destination.h
//
// $Id: //poco/Main/PDF/include/Poco/PDF/Destination.h#4 $
//
// Library: PDF
// Package: PDFCore
// Module:  Destination
//
// Definition of the Destination class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef PDF_Destination_INCLUDED
#define PDF_Destination_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/PDF/Resource.h"


namespace Poco {
namespace PDF {


class PDF_API Destination: public Resource<HPDF_Destination>
	/// Destination class represents destination resource.
{
public:
	Destination(HPDF_Doc* pPDF, const HPDF_Destination& resource, const std::string& name = "");
		/// Creates the destination.

	~Destination();
		/// Destroys the destination.

	void positionAndZoom(float x, float y, float zoom);
		/// Sets the position and zoom for destination.

	void fit();
		/// Sets the appearance of the page to displaying entire page within the window. 

	void fitHorizontal(float top);
		/// Defines the appearance of a page to magnifying to fit the height of the
		/// page within the window and setting the top position of the page to the
		/// value of the "top" parameter.

	void fitVertical(float left);
		/// Defines the appearance of a page to magnifying to fit the height of the
		/// page within the window and setting the left position of the page to the
		/// value of the "left" parameter.

	void fitRectangle(float left, float top, float right, float bottom);
		/// Defines the appearance of a page to magnifying the page to fit a rectangle
		/// specified by left, bottom, right and top.

	void fitWindow();
		/// Sets the appearance of the page to magnifying to fit  the bounding box of
		/// the page within the window. 

	void fitWindowHorizontal(float top);
		/// Defines the appearance of a page to magnifying to fit the width of the
		/// bounding box of the page within the window and setting the top position
		/// of the page to the value of the "top" parameter.

	void fitWindowVertical(float left);
		/// Defines the appearance of a page to magnifying to fit the height of the
		/// bounding box of the page within the window and setting the left position
		/// of the page to the value of the "left" parameter.
};


//
// inlines
//

inline void Destination::positionAndZoom(float x, float y, float zoom)
{
	HPDF_Destination_SetXYZ(handle(), x, y, zoom);
}


inline void Destination::fit()
{
	HPDF_Destination_SetFit(handle());
}


inline void Destination::fitHorizontal(float top)
{
	HPDF_Destination_SetFitH(handle(), top);
}


inline void Destination::fitVertical(float left)
{
	HPDF_Destination_SetFitV(handle(), left);
}


inline void Destination::fitRectangle(float left, float top, float right, float bottom)
{
	HPDF_Destination_SetFitR(handle(), left, bottom, right, top);
}


inline void Destination::fitWindow()
{
	HPDF_Destination_SetFitB(handle());
}


inline void Destination::fitWindowHorizontal(float top)
{
	HPDF_Destination_SetFitBH(handle(), top);
}


inline void Destination::fitWindowVertical(float left)
{
	HPDF_Destination_SetFitBV(handle(), left);
}


} } // namespace Poco::PDF


#endif // PDF_Destination_INCLUDED
