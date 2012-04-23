//
// Event.cpp
//
// $Id: //poco/1.4/XML/src/Event.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOMEvents
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/DOM/Event.h"
#include "Poco/DOM/Document.h"


namespace Poco {
namespace XML {


Event::Event(Document* pOwnerDocument, const XMLString& type):
	_pOwner(pOwnerDocument),
	_type(type),
	_pTarget(0),
	_pCurrentTarget(0),
	_currentPhase(CAPTURING_PHASE),
	_bubbles(true),
	_cancelable(true),
	_canceled(false),
	_stopped(false)
{
}


Event::Event(Document* pOwnerDocument, const XMLString& type, EventTarget* pTarget, bool canBubble, bool isCancelable):
	_pOwner(pOwnerDocument),
	_type(type),
	_pTarget(pTarget),
	_pCurrentTarget(0),
	_currentPhase(CAPTURING_PHASE),
	_bubbles(canBubble),
	_cancelable(isCancelable),
	_canceled(false),
	_stopped(false)
{
}


Event::~Event()
{
}


void Event::stopPropagation()
{
	_stopped = true;
}


void Event::preventDefault()
{
	_canceled = true;
}


void Event::initEvent(const XMLString& eventType, bool canBubble, bool isCancelable)
{
	_type       = eventType;
	_bubbles    = canBubble;
	_cancelable = isCancelable;
	_canceled   = false;
	_stopped    = false;
}


void Event::setTarget(EventTarget* pTarget)
{
	_pTarget = pTarget;
}


void Event::setCurrentPhase(PhaseType phase)
{
	_currentPhase = phase;
}


void Event::setCurrentTarget(EventTarget* pTarget)
{
	_pCurrentTarget = pTarget;
}


void Event::autoRelease()
{
	_pOwner->autoReleasePool().add(this);
}


} } // namespace Poco::XML
